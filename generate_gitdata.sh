#!/bin/bash

# Define output file
OUTPUT_FILE="gitdata.h"

# Get git information
COMMIT_HASH=$(git rev-parse HEAD)
BRANCH_NAME=$(git rev-parse --abbrev-ref HEAD)
REPO_URL=$(git config --get remote.origin.url)

# Create the header file
cat <<EOL > $OUTPUT_FILE
#ifndef GITDATA_H
#define GITDATA_H

#define GIT_COMMIT_HASH "$COMMIT_HASH"
#define GIT_BRANCH "$BRANCH_NAME"
#define GIT_REPO_URL "$REPO_URL"

#endif // GITDATA_H
EOL

echo "$OUTPUT_FILE has been generated."
