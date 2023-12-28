#include "Track.h"

#include "raymath.h"

static float track_progress = 0;
static int track_amount = 0;

static int end_track = MAX_TRACK_SEGMENTS - 1;

static Vector3 addOffset = { 10, 0, 0 };

static Vector3 segments[MAX_TRACK_SEGMENTS];

static TrackCursor cursors[MAX_CURSORS];

Model trainModel;

void initTrack(Model* model)
{
    trainModel = *model;
    Vector3 lastTrack = (Vector3){ -1 , 0 , -1 };
    //load default startup track.
    for (int i = 0; i < MAX_TRACK_SEGMENTS; ++i)
    {
        Vector3 seg = Vector3Add(lastTrack, addOffset);
        lastTrack = seg;
        segments[i] = seg;
        track_amount++;
    }

    for (int i = 0; i < MAX_CURSORS; ++i)
    {
        TrackCursor cursor;
        cursor.endSeg = i + 5;
        cursor.progress = 0.5f;
        cursors[i] = cursor;
    }

}

//draws all track segments.
void renderTrack()
{
    for (int i = 0; i < track_amount; i++)
    {

        DrawSphere(segments[i], 0.5f, GREEN);
        if (i != track_amount - 1)
        {
            DrawLine3D(segments[i], segments[i + 1], RED);
        }
    }
}

void updatePosition()
{

    size_t cursorSize = sizeof(cursors) / sizeof(TrackCursor);

    //loop through all the cursors.
    for (int i = 0; i < cursorSize; ++i)
    {
        //increment progress.
        //updates the end segment if reaches the end of a segment.
        //if at the end of the track, reset to the start (In future will never reach the end as it will extend past the train).
        if (cursors[i].progress >= 100)
        {
            if (cursors[i].endSeg == MAX_TRACK_SEGMENTS - 1)
            {
                cursors[i].endSeg = 0;
                cursors[i].progress = 0;
            }
            else
            {
                cursors[i].endSeg++;
                cursors[i].progress = 0;
            }
        }
        cursors[i].progress++;

        int startIndex = cursors[i].endSeg - 1;
        if (startIndex < 0)
        {
            startIndex = MAX_TRACK_SEGMENTS - 1;
        }

        Vector3 position = Vector3Lerp(segments[startIndex], segments[cursors[i].endSeg], cursors[i].progress / 100);
        DrawSphere(position, 2.0f, RED);
        DrawModel(trainModel, position, 2.0f, RED);

        //Draw debug sphere
    }
}

void extendTrack()
{
    TraceLog(LOG_INFO, "Extending track.");
    int lastEnd = end_track;
    if (end_track == MAX_TRACK_SEGMENTS - 1)
    {
        end_track = 0;
    }
    else
    {
        end_track++;
    }
    segments[end_track] = Vector3Add(segments[lastEnd], addOffset);

    size_t cursorSize = sizeof(cursors) / sizeof(TrackCursor);

    //loop through all the cursors.
    for (int i = 0; i < cursorSize; ++i)
    {
        //if (cursors[i].endSeg == )
    }


    TraceLog(LOG_INFO, "End track: %i", end_track);

}

Vector3 trackPositionToWorld(float position)
{
    return (Vector3){ 0 };
}
