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

struct Train
{
	bool isMoving;
	float moveSpeed;
	TrackCursor carriages[MAX_CURSORS];
	int carriageAmount;

};


void initTrack(Model* model);
void renderTrain();
void renderTrack();
void updatePosition();
void extendTrack(Vector3 point);
void extendRandom();


//moves x meters from a current position.

Vector3 TrackToWorld(TrackCursor cursor);
TrackCursor GetTrackForward(TrackCursor cursor, float distance);

int getLastTrack(int trackIndex);
int getNextTrack(int trackIndex);

void trackDebug();