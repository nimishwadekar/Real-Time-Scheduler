#include "frame.h"
#include <stdlib.h>

struct Frame
{
	PTaskArrayPtr ptasks;
	// Sporadic tasks
	// Aperiodic tasks
};


FramePtr Frame_new(PTaskArrayPtr ptasks)
{
	FramePtr frame = malloc(sizeof(struct Frame));
	if(!frame)
	{
		return NULL;
	}

	frame->ptasks = ptasks;
	return frame;
}


void Frame_delete(FramePtr frame)
{
	free(frame);
}


PTaskArrayPtr Frame_get_ptasks(FramePtr frame)
{
	return frame->ptasks;
}