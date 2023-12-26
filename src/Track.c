#include "Track.h"

#include "raymath.h"

void initTrack(Track* track)
{
    track->progress = 0;
    track->trackAmount = 0;
    track->startTrack = 0;
    track->endTrack = 1;
    for (int i = 0; i < MAX_TRACK_SEGMENTS; ++i)
    {
        TrackSegment* trackSeg = (TrackSegment*)malloc(sizeof(TrackSegment));
        trackSeg->point = (Vector3){ i, 0, i };
        track->TrackSegments[i] = trackSeg;
        track->trackAmount++;
    }
}

Vector3 updatePosition(Track* track)
{
    if (track->progress > 100)
    {
        if (track->endTrack + 1 == track->trackAmount)
        {
            track->endTrack = 1;
            track->startTrack = 0;
        }
        else
        {
            track->endTrack++;
            track->startTrack++;
        }
        track->progress = 0;
    }
    return Vector3Lerp(track->TrackSegments[track->startTrack]->point, track->TrackSegments[track->endTrack]->point, track->progress++ / 100);
}
