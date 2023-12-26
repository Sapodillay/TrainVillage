#include <raylib.h>
#include <stdlib.h>

#define MAX_TRACK_SEGMENTS 20

typedef struct TrackSegment TrackSegment;

typedef struct Track Track;

struct TrackSegment
{
	Vector3 point;
};

struct Track
{
	TrackSegment* TrackSegments[MAX_TRACK_SEGMENTS];
	int trackAmount;
	float progress;
	int startTrack;
	int endTrack;

};

void initTrack(Track* track);
Vector3 updatePosition(Track* track);
