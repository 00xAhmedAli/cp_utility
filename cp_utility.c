#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int cp_main(int argc, char *argv[]) {
    // Check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *source_path = argv[1];
    const char *dest_path = argv[2];
    
    int src_fd = -1;
    int dest_fd = -1;
    struct stat src_stat;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    
    // Open source file for reading
    src_fd = open(source_path, O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, "Error opening source file '%s': %s\n", 
                source_path, strerror(errno));
        return EXIT_FAILURE;
    }
    
    // Get source file permissions
    if (fstat(src_fd, &src_stat) == -1) {
        fprintf(stderr, "Error getting file stats for '%s': %s\n", 
                source_path, strerror(errno));
        close(src_fd);
        return EXIT_FAILURE;
    }
    
    // Open destination file for writing
    // Use same permissions as source file, create if doesn't exist, truncate if exists
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, src_stat.st_mode);
    if (dest_fd == -1) {
        fprintf(stderr, "Error opening destination file '%s': %s\n", 
                dest_path, strerror(errno));
        close(src_fd);
        return EXIT_FAILURE;
    }
    
    // Copy data from source to destination
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        
        if (bytes_written != bytes_read) {
            fprintf(stderr, "Error writing to destination file: %s\n", 
                    strerror(errno));
            close(src_fd);
            close(dest_fd);
            return EXIT_FAILURE;
        }
    }
    
    // Check for read error
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from source file: %s\n", 
                strerror(errno));
        close(src_fd);
        close(dest_fd);
        return EXIT_FAILURE;
    }
    
    // Close files
    if (close(src_fd) == -1) {
        fprintf(stderr, "Error closing source file: %s\n", strerror(errno));
        close(dest_fd);
        return EXIT_FAILURE;
    }
    
    if (close(dest_fd) == -1) {
        fprintf(stderr, "Error closing destination file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}