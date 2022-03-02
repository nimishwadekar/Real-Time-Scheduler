#include "inf.h"
#include "common.h"
#include "ptask.h"
#include "queue.h"
#include <string.h>
#include <float.h>
#include <stdio.h>

struct INFInfo
{
	size_t vertex_count;
	size_t job_offset;
	size_t ptask_count;
	size_t total_job_count;
	size_t *job_counts;
	size_t frame_offset;
	size_t frame_count;
	size_t frame_size;
};


static void print_graph(size_t vertex_count, double graph[vertex_count][vertex_count])
{
	printf("    ");
	for(size_t i = 0; i < vertex_count; i++)
		printf("%2lu ", i);
	printf("\n\n");
	for(size_t i = 0; i < vertex_count; i++)
	{
		printf("%2lu  ", i);
		for(size_t j = 0; j < vertex_count; j++)
		{
			printf("%2g ", graph[i][j]);
		}
		printf("\n");
	}
}


static void initialise_graph(size_t vertex_count, double graph[vertex_count][vertex_count], struct INFInfo *infInfo, PTaskArrayPtr array)
{
	memset(graph, 0, vertex_count * vertex_count * sizeof(double));

	// Initialise source to jobs edges. capacity = execution time of job.
	size_t last_task_offset = infInfo->job_offset;
	for(size_t i = 0; i < infInfo->ptask_count; i++)
	{
		for(size_t v = last_task_offset; v < infInfo->job_offset + infInfo->job_counts[i]; v++)
		{
			graph[0][v] = PTask_get_execution_time(PTaskArray_get(array, i));
		}
		last_task_offset = infInfo->job_offset + infInfo->job_counts[i];
	}


	// Initialise frames to sink edges. capacity = frame size.
	for(size_t i = infInfo->frame_offset; i < infInfo->frame_offset + infInfo->frame_count; i++)
	{
		graph[i][vertex_count - 1] = infInfo->frame_size;
	}


	// Initialise jobs to frames edges. capacity = frame size.
	last_task_offset = infInfo->job_offset;
	for(size_t i = 0; i < infInfo->ptask_count; i++)
	{
		PTaskPtr task = PTaskArray_get(array, i);
		for(size_t v = last_task_offset, job_number = 0; v < infInfo->job_offset + infInfo->job_counts[i]; v++, job_number++)
		{
			int release_time = PTask_get_release_time(task, job_number);
			int absolute_deadline = PTask_get_absolute_deadline(task, job_number);

			for(size_t f = (release_time + infInfo->frame_size - 1) / infInfo->frame_size; 
				f < absolute_deadline / infInfo->frame_size; f++)
			{
				graph[v][infInfo->frame_offset + f] = infInfo->frame_size;
			}
		}
		last_task_offset = infInfo->job_offset + infInfo->job_counts[i];
	}
}


static double edmonds_karp(size_t vertex_count, double capacity_graph[vertex_count][vertex_count], double flow_graph[vertex_count][vertex_count])
{
	double flow = 0;
	while(1)
	{
		int from[vertex_count];
		for(size_t i = 0; i < vertex_count; i++)
		{
			from[i] = -1;
		}

		QueueIntPtr queue = QueueInt_new(vertex_count);
		if(!queue)
		{
			fprintf(stderr, "edmonds_karp(): queue creation failed\n");
			return -1;
		}

		QueueInt_enqueue(queue, 0);

		while(!QueueInt_is_empty(queue)) // Perform BFS to find shortest augmenting path to sink.
		{
			int u = QueueInt_dequeue(queue);
			for(size_t v = 0; v < vertex_count; v++)
			{
				if(!DOUBLE_EQ(capacity_graph[u][v], 0) && from[v] == -1 &&
					v != 0 && capacity_graph[u][v] > flow_graph[u][v])
				{
					from[v] = u;
					QueueInt_enqueue(queue, v);
				}
			}
		}

		if(from[vertex_count - 1] != -1) // Found an augmenting path to sink.
		{
			double delta_flow = DBL_MAX;
			for(int u = from[vertex_count - 1], v = vertex_count - 1; u != -1; )
			{
				double flow_space = capacity_graph[u][v] - flow_graph[u][v];
				delta_flow = (delta_flow < flow_space) ? delta_flow : flow_space;
				v = u;
				u = from[u];
			}

			for(int u = from[vertex_count - 1], v = vertex_count - 1; u != -1; )
			{
				flow_graph[u][v] += delta_flow;
				flow_graph[v][u] -= delta_flow;
				v = u;
				u = from[u];
			}
			flow += delta_flow;
		}
		else
		{
			break;
		}
	}

	//print_graph(vertex_count, flow_graph);
	return flow;
}

int inf_algorithm(PTaskArrayPtr array, int frame_size, int frame_count, size_t total_jobs, struct FramePJobEntry entries[frame_count][total_jobs])
{
	size_t ptask_count = PTaskArray_get_size(array);
	size_t job_counts[ptask_count];
	PTaskArray_get_jobs_per_hyper_period(array, job_counts);
	
	for(size_t i = 1; i < ptask_count; i++) // Cumulative job counts for convenience.
	{
		job_counts[i] += job_counts[i - 1];
	}

	// Vertex Count = Source  + Jobs + Frames + Sink.
	size_t vertex_count = 1 + job_counts[ptask_count - 1] + frame_count + 1;

	struct INFInfo infInfo;
	infInfo.vertex_count = vertex_count;
	infInfo.job_offset = 1;
	infInfo.ptask_count = ptask_count;
	infInfo.total_job_count = job_counts[ptask_count - 1];
	infInfo.job_counts = job_counts;
	infInfo.frame_offset = 1 + job_counts[ptask_count - 1];
	infInfo.frame_count = frame_count;
	infInfo.frame_size = frame_size;


	// Flow capacity adjacency graph. Vertex 0 is source vertex. Vertices 1 to total job count (inclusive) are job vertices.
	// Vertices (1 + total job count) to (1 + total job count + frame count) are frame vertices. Last vertex is sink vertex.
	double capacity_graph[vertex_count][vertex_count];
	initialise_graph(vertex_count, capacity_graph, &infInfo, array);

	double total_execution_time = 0;
	for(size_t i = 0; i < PTaskArray_get_size(array); i++)
	{
		PTaskPtr task = PTaskArray_get(array, i);
		total_execution_time += PTask_get_execution_time(task) *
			(frame_count * frame_size) / PTask_get_period(task);
	}

	double flow_graph[vertex_count][vertex_count];
	memset(flow_graph, 0, sizeof(flow_graph));

	if(!DOUBLE_EQ(edmonds_karp(vertex_count, capacity_graph, flow_graph), total_execution_time))
	{
		return -1;
	}

	// Fill in entries array.

	for(size_t f = 0; f < frame_count; f++)
	{
		size_t last_task_offset = 0;
		for(size_t t = 0; t < PTaskArray_get_size(array); t++)
		{
			size_t job_count = job_counts[t];
			if(t > 0) job_count -= job_counts[t - 1];

			for(size_t j = 0; j < job_count; j++)
			{
				double exec_time;
				if((exec_time = flow_graph[infInfo.job_offset + last_task_offset + j][infInfo.frame_offset + f]) == 0)
				{
					continue;
				}

				struct FramePJobEntry *entry = &entries[f][last_task_offset + j];
				entry->valid = 1;
				entry->task = t;
				entry->job = j;
				entry->exec_time = exec_time;
			}
			last_task_offset = infInfo.job_counts[t];
		}
	}

	return 0;
}