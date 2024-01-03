#include "Track.h"

#include "raymath.h"

static float track_progress = 0;
static int track_amount = 0;

static int end_track = 0;

static Vector3 addOffset = { 10, 0, 0 };
static Vector3 addOffset2 = { 20, 0, 0 };


static TrackSegment segments[MAX_TRACK_SEGMENTS];
static float segmentLength[MAX_TRACK_SEGMENTS];

static float distanceTest = 0;

Train train = { 0 };

Vector3 curvePoints[10];


Model trainModel;

void initTrack(Model* model)
{
    trainModel = *model;
    train.isMoving = true;
    train.moveSpeed = 0.1f;
    train.padding = 1.0f;

    train.startCursor.endSeg = 5;
    train.startCursor.progress = 0.5f;

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

    float distanceFromTrain = 0.0f;

    for (int i = 0; i < MAX_CURSORS; ++i)
    {
        TrackCursor cursor;
        cursor.endSeg = train.startCursor.endSeg;
        cursor.progress = train.startCursor.progress;
        cursor = GetTrackForward(cursor, distanceFromTrain);

        train.carriageAmount++;
        train.carriages[i].size = 7.5f;
        train.carriages[i].cursor = cursor;

        distanceFromTrain += train.carriages[i].size;
        distanceFromTrain += train.padding;

    }

}

//render trains
void renderTrain()
{
    //loop through all the cursors.
    for (int i = 0; i < train.carriageAmount; ++i)
    {
        Vector3 position = TrackToWorld(train.carriages[i].cursor);
        GetForwardVector(train.carriages[i].cursor);
        DrawModel(trainModel, position, 2.0f, RED);
    }
}

//draws all track segments.
void renderTrack()
{
    drawDebugTrack();
    
}

void updatePosition()
{


    //loop through all the cursors.
    for (int i = 0; i < train.carriageAmount; ++i)
    {
        //increment progress.
        //updates the end segment if reaches the end of a segment.
        //if at the end of the track, reset to the start (In future will never reach the end as it will extend past the train).
        if (train.carriages[i].cursor.progress >= 1)
        {
            train.carriages[i].cursor.endSeg = getNextTrack(train.carriages[i].cursor.endSeg);
            train.carriages[i].cursor.progress = 0;
        }

        train.carriages[i].cursor.progress += (10.0f / segmentLength[train.carriages[i].cursor.endSeg]) / 100;
    }
}

void extendTrack(Vector3 point)
{

    end_track = getNextTrack(end_track); 


    if (track_amount < MAX_TRACK_SEGMENTS) track_amount++;

    TrackSegment newSegment = { 0 };
    newSegment.segment = point;

    segments[end_track] = newSegment;
    if (track_amount == 1)
    {
        segmentLength[end_track] = Vector3Distance((Vector3){0}, segments[end_track].segment);
    }
    else
    {
        segmentLength[end_track] = Vector3Distance(segments[getLastTrack(end_track)].segment, segments[end_track].segment);
    }
    TraceLog(LOG_INFO, "Extending track. %i", end_track);
    TraceLog(LOG_INFO, "Distance:  %f", segmentLength[end_track]);

}

void extendRandom()
{

    Vector3 randomOffset = (Vector3){ GetRandomValue(1, 30), 0, 0};
    Vector3 point = Vector3Add(segments[end_track].segment, randomOffset);
    extendTrack(point);
}

void extendCurve()
{

    TrackSegment newSegment = { 0 };
    newSegment.segment = Vector3Add(segments[end_track].segment, (Vector3){ 20, 0, 20 });
    newSegment.isCurve = true;
    newSegment.curve = Vector3Add(segments[end_track].segment, (Vector3) { 20, 0, 0 });

    end_track = getNextTrack(end_track);


    if (track_amount < MAX_TRACK_SEGMENTS) track_amount++;


    segments[end_track] = newSegment;
    if (track_amount == 1)
    {
        segmentLength[end_track] = Vector3Distance((Vector3) { 0 }, segments[end_track].segment);
    }
    else
    {
        segmentLength[end_track] = Vector3Distance(segments[getLastTrack(end_track)].segment, segments[end_track].segment);
    }
    TraceLog(LOG_INFO, "Extending track. %i", end_track);
    TraceLog(LOG_INFO, "Distance:  %f", segmentLength[end_track]);

}

void drawDebugTrack()
{
    int currentTrack = getNextTrack(end_track);
    for (int i = 0; i < track_amount; i++)
    {
        currentTrack = getNextTrack(currentTrack);
        DrawSphere(segments[currentTrack].segment, 0.5f, GREEN);
        if (currentTrack != end_track)
        {
            //draw curve.
            if (segments[getNextTrack(currentTrack)].isCurve)
            {
                Vector3 startSeg = segments[currentTrack].segment;
                Vector3 curveSegment = segments[getNextTrack(currentTrack)].curve;
                Vector3 nextSegment = segments[getNextTrack(currentTrack)].segment;

                int count = 0;
                for (float i = 0; i < 1.0; i += 0.1f)
                {
                    count++;
                    Vector3 pos = BezierCurveLerp(startSeg, curveSegment, nextSegment, i);
                    curvePoints[count] = pos;
                }
                for (int i = 0; i < 10; i++)
                {
                    if (i != count - 3)
                    {
                        DrawLine3D(curvePoints[i], curvePoints[i + 1], YELLOW);
                    }

                }
                //draw handle.
                DrawSphere(segments[getNextTrack(currentTrack)].curve, 0.7f, BLACK);
            }

            DrawLine3D(segments[currentTrack].segment, segments[getNextTrack(currentTrack)].segment, RED);
        }
    }
}


Vector3 BezierCurveLerp(Vector3 p1, Vector3 p2, Vector3 p3, float progress)
{
    Vector3 lerp1 = Vector3Lerp(p1, p2, progress);
    Vector3 lerp2 = Vector3Lerp(p2, p3, progress);
    return Vector3Lerp(lerp1, lerp2, progress);

}

Vector3 TrackToWorld(TrackCursor cursor)
{
    int startSeg = getLastTrack(cursor.endSeg);
    if (segments[cursor.endSeg].isCurve)
    {
        return BezierCurveLerp(segments[startSeg].segment, segments[cursor.endSeg].curve, segments[cursor.endSeg].segment, cursor.progress);
    }
    return Vector3Lerp(segments[startSeg].segment, segments[cursor.endSeg].segment, cursor.progress);
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

Vector3 GetForwardVector(TrackCursor cursor)
{
    //find forward 
    int startSeg = getLastTrack(cursor.endSeg);
    int endSeg = cursor.endSeg;

    Vector3 position = { 0 };
    Vector3 forward = { 0 };

    if (segments[endSeg].isCurve)
    {
        position = BezierCurveLerp(segments[startSeg].segment, segments[endSeg].curve, segments[endSeg].segment, cursor.progress);
        forward = BezierCurveLerp(segments[startSeg].segment, segments[endSeg].curve, segments[endSeg].segment, cursor.progress + 0.01f);
    }
    else
    {
        position = Vector3Lerp(segments[startSeg].segment, segments[endSeg].segment, cursor.progress);
        forward = Vector3Lerp(segments[startSeg].segment, segments[endSeg].segment, cursor.progress + 0.01f);
    }

    Vector3 direction = Vector3Subtract(forward, position);
    direction = Vector3Normalize(direction);

    //debug draw
    direction = Vector3Scale(direction, 10.0f);
    direction = Vector3Add(direction, (Vector3) { 0, 1, 0 });
    DrawLine3D(position, Vector3Add(position, direction), ORANGE);
    return direction;
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

