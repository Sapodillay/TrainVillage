#include <raylib.h>
#include <stdlib.h>

#define MAX_CURSORS 5
#define MAX_TRACK_SEGMENTS 20 + MAX_CURSORS

typedef struct TrackCursor TrackCursor;

struct TrackCursor
{
	int endSeg;
	float progress;
};

void initTrack();
void renderTrack();
void updatePosition();
void extendTrack();
