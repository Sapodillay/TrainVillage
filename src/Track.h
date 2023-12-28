#include <raylib.h>
#include <stdlib.h>

#define MAX_CURSORS 5
#define MAX_TRACK_SEGMENTS 20 + MAX_CURSORS

typedef struct TrackCursor TrackCursor;
typedef struct Train Train;


struct TrackCursor
{
	int endSeg;
	float progress;
};

void initTrack(Model* model);
void renderTrack();
void updatePosition();
void extendTrack();
