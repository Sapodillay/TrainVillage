#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"

#include "EntityManager.h"
#include "Track.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;




    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    DisableCursor();

    Model train = LoadModel("../../../src/train.obj");
    Texture2D texture = LoadTexture("../../../src/Train_texture.png");
    train.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    //test entitymanager


    EntityManager entityManger;

    initEntityManger(&entityManger, 10);
    addEntity(&entityManger);

    //load default track
    initTrack(&train);

    



    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------

        Vector3 moveVector = { 0 };
        if (IsKeyDown(KEY_A))
        {
            moveVector.x -= 1.0f;
        }
        if (IsKeyDown(KEY_D))
        {
            moveVector.x += 1.0f;
        }
        if (IsKeyDown(KEY_W))
        {
            moveVector.y += 1.0f;
        }
        if (IsKeyDown(KEY_S))
        {
            moveVector.y -= 1.0f;
        }

        if (IsKeyDown(KEY_Q))
        {
            moveVector.z += 1.0f;
        }
        if (IsKeyDown(KEY_E))
        {
            moveVector.z -= 1.0f;
        }
        if (IsKeyPressed(KEY_C))
        {
            extendTrack();
        }


        moveVector = Vector3Normalize(moveVector);
        CameraMoveForward(&camera, moveVector.y, true);
        CameraMoveRight(&camera, moveVector.x, true);
        CameraMoveUp(&camera, moveVector.z, true);



        Vector2 mouseDelta = GetMouseDelta();

        CameraYaw(&camera, -(mouseDelta.x * 0.01), false);
        CameraPitch(&camera, -(mouseDelta.y * 0.01), false, false, false);



        BeginDrawing();
        ClearBackground(RAYWHITE);


        BeginMode3D(camera);



        //DrawModel(train, (Vector3) { 0, 0, 0 }, 5.0f, WHITE);

        renderTrack();
        updatePosition();

        DrawGrid(1280.0f, 8.0f);
        EndMode3D();

        DrawFPS(20, 20);
        DrawText("test", 10, 10, 5, RED);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadModel(apple);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
