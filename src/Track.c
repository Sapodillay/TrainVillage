#include "Track.h"

#include "raymath.h"

static float track_progress = 0;
static int track_amount = 0;

static int end_track = 0;

static Vector3 addOffset = { 10, 0, 0 };
static Vector3 addOffset2 = { 20, 0, 0 };


static Vector3 segments[MAX_TRACK_SEGMENTS];
static float segmentLength[MAX_TRACK_SEGMENTS];

static float distanceTest = 0;

Train train = { 0 };


Model trainModel;

void initTrack(Model* model)
{
    trainModel = *model;
    train.isMoving = true;
    train.moveSpeed = 0.1f;

    Vector3 lastTrack = (Vector3){ 0 , 0 , 0 };
    bool firstTrack = true;
    //load default startup track.
    for (int i = 0; i < MAX_TRACK_SEGMENTS; ++i)
    {
        Vector3 point = { 0 };
        if (firstTrack)
        {
            point = lastTrack;
            firstTrack = false;
        }
        else
        {
            if (i % 2 == 0)
            {
                point = Vector3Add(lastTrack, addOffset2);

            }
            else
            {
                point = Vector3Add(lastTrack, addOffset);
            }
        }
        extendTrack(point);
        lastTrack = point;
    }

    for (int i = 0; i < MAX_CURSORS; ++i)
    {
        TrackCursor cursor;
        cursor.endSeg = i + 5;
        cursor.progress = 0.5f;
        train.carriageAmount++;
        train.carriages[i] = cursor;
    }

}

//render trains
void renderTrain()
{
    //loop through all the cursors.
    for (int i = 0; i < train.carriageAmount; ++i)
    {
        Vector3 position = TrackToWorld(train.carriages[i]);
        DrawModel(trainModel, position, 2.0f, RED);
    }
}

//draws all track segments.
void renderTrack()
{


    //debug draw distance
    TrackCursor testCursor = { 0 };
    testCursor.endSeg = 3;
    testCursor.progress = 0.0f;
    TrackCursor result = GetTrackForward(testCursor, distanceTest);

    Vector3 position = TrackToWorld(result);
    DrawSphere(position, 1.0f, ORANGE);

    
    int currentTrack = getNextTrack(end_track);
    for (int i = 0; i < track_amount; i++)
    {
        currentTrack = getNextTrack(currentTrack);
        DrawSphere(segments[currentTrack], 0.5f, GREEN);
        if (currentTrack != end_track)
        {
            DrawLine3D(segments[currentTrack], segments[getNextTrack(currentTrack)], RED);
        }
    }
}

void updatePosition()
{


    //loop through all the cursors.
    for (int i = 0; i < train.carriageAmount; ++i)
    {
        //increment progress.
        //updates the end segment if reaches the end of a segment.
        //if at the end of the track, reset to the start (In future will never reach the end as it will extend past the train).
        if (train.carriages[i].progress >= 1)
        {
            train.carriages[i].endSeg = getNextTrack(train.carriages[i].endSeg);
            train.carriages[i].progress = 0;
        }

        train.carriages[i].progress += (10.0f / segmentLength[train.carriages[i].endSeg]) / 100;
    }
}

void extendTrack(Vector3 point)
{

    end_track = getNextTrack(end_track); 


    if (track_amount < MAX_TRACK_SEGMENTS) track_amount++;

    segments[end_track] = point;
    if (track_amount == 1)
    {
        segmentLength[end_track] = Vector3Distance((Vector3){0}, segments[end_track]);
    }
    else
    {
        segmentLength[end_track] = Vector3Distance(segments[getLastTrack(end_track)], segments[end_track]);
    }
    TraceLog(LOG_INFO, "Extending track. %i", end_track);
    TraceLog(LOG_INFO, "Distance:  %f", segmentLength[end_track]);

}

void extendRandom()
{

    Vector3 randomOffset = (Vector3){ GetRandomValue(1, 30), 0, 0};
    Vector3 point = Vector3Add(segments[end_track], randomOffset);
    extendTrack(point);
}


Vector3 TrackToWorld(TrackCursor cursor)
{
    int startSeg = getLastTrack(cursor.endSeg);
    return Vector3Lerp(segments[startSeg], segments[cursor.endSeg], cursor.progress);
}

TrackCursor GetTrackForward(TrackCursor cursor, float distance)
{
    //current segment cursor is on.
    int segment = getLastTrack(cursor.endSeg);

    //distance of the segment in world space.
    float segDistance = segmentLength[segment];
    //current distance into the segment in world space.
    float currentDistance = segDistance * cursor.progress;
    //distance to the end of the segment.
    float distanceNeeded = segDistance - currentDistance;


    //add distance that is needed in world space.
    currentDistance += distance;
    //calculate the new perctange.
    float newProgress = currentDistance / segDistance;
    TrackCursor result = { 0 };

    //if percentage is lower than 1, it is in the same segment. return new progress.
    if (newProgress < 1)
    {
        //is in current segment
        result.endSeg = segment;
        result.progress = newProgress;
    }
    else // else further calculate the next segments.
    {
        newProgress = newProgress - 1;
        float remainingDistance = distance - distanceNeeded;
        //create new cursor
        TrackCursor newCursor = { 0 };
        newCursor.endSeg = getNextTrack(cursor.endSeg);

        result.progress = newProgress;
        result = GetTrackForward(newCursor, remainingDistance);
    }

    return result;
}

int getLastTrack(int trackIndex)
{
    if (trackIndex == 0)
    {
        return MAX_TRACK_SEGMENTS - 1;
    }
    else
    {
        return trackIndex - 1;
    }
}

int getNextTrack(int trackIndex)
{
    if (trackIndex == MAX_TRACK_SEGMENTS - 1)
    {
        return 0;
    }
    else
    {
        return trackIndex + 1;
    }
}

void trackDebug()
{
    distanceTest += 5.0f;
}

