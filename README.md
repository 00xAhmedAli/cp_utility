Key features of this implementation:

    Proper argument checking: Ensures exactly 2 arguments are provided

    Error handling: Checks all system calls and provides meaningful error messages

    File permissions: Preserves the source file's permissions in the destination

    Efficient copying: Uses a buffer to copy data in chunks

    Proper resource management: Ensures files are closed even on errors

    Input validation: Checks if source is a regular file
