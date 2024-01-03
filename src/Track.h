#include <raylib.h>
#include <stdlib.h>

#define MAX_CURSORS 5
#define MAX_TRACK_SEGMENTS 20 + MAX_CURSORS

typedef struct TrackCursor TrackCursor;
typedef struct Train Train;
typedef struct Carriage Carriage;
typedef struct TrackSegment TrackSegment;

struct TrackSegment
{
	Vector3 segment;
	bool isCurve;
	Vector3 curve;
};

struct TrackCursor
{
	int endSeg;
	float progress;
};

struct Carriage
{
	float size;
	TrackCursor cursor;

};

struct Train
{
	bool isMoving;
	float moveSpeed;
	Carriage carriages[MAX_CURSORS];
	float padding;
	int carriageAmount;

	TrackCursor startCursor;
};
//Initalize
void initTrack(Model* model);

//Rendering
void renderTrain();
void renderTrack();
void drawDebugTrack();

void updatePosition();

void extendTrack(Vector3 point);
void extendRandom();
void extendCurve();




Vector3 BezierCurveLerp(Vector3 p1, Vector3 p2, Vector3 p3, float progress);
Vector3 TrackToWorld(TrackCursor cursor);
TrackCursor GetTrackForward(TrackCursor cursor, float distance);
Vector3 GetForwardVector(TrackCursor cursor);

int getLastTrack(int trackIndex);
int getNextTrack(int trackIndex);

void trackDebug();