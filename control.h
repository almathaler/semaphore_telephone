#ifndef control_h
#define control_h
/*
Take a command line argument to determine if it is creating or removing the resources or viewing the story.
If creating (command line argument: -c):
    Should make the shared memory, semaphore and file (open the file with the truncate flag).
    Set any values that are needed.
If removing (command line argument: -r)
    Remove the shared memory, the semaphore and the story.
    Display the full contents of the story.
    This should wait until the semaphore is available.
If viewing (command line argument: -v)
    Output the contents of the story file.
    This mode does not need to interact with the semaphore
*/
//gonna return 0, 1 or 2 depending on the command line arg
int crv(char* cmd_line);
//for creating -- 0 on success
int c();
//for removing -- 0 on success
int r();
//for viewing -- 0 on succes
int v();

#endif
