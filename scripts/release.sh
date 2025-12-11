#!/bin/bash

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored messages
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if version is provided
if [ -z "$1" ]; then
    print_error "Version number required"
    echo "Usage: $0 <version>"
    echo "Example: $0 1.0.0"
    exit 1
fi

VERSION=$1

# Validate version format (semver)
if ! [[ $VERSION =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    print_error "Invalid version format. Use semantic versioning (e.g., 1.0.0)"
    exit 1
fi

TAG="v${VERSION}"

print_info "Creating release ${TAG}"

# Check if tag already exists
if git rev-parse "$TAG" >/dev/null 2>&1; then
    print_error "Tag ${TAG} already exists"
    exit 1
fi

# Check if working directory is clean
if [ -n "$(git status --porcelain)" ]; then
    print_warn "Working directory is not clean"
    git status --short
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Aborted"
        exit 0
    fi
fi

# Ensure we're on main branch
CURRENT_BRANCH=$(git branch --show-current)
if [ "$CURRENT_BRANCH" != "main" ]; then
    print_warn "Not on main branch (currently on ${CURRENT_BRANCH})"
    read -p "Continue anyway? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        print_info "Aborted"
        exit 0
    fi
fi

# Pull latest changes
print_info "Pulling latest changes..."
git pull origin main

# Update version in README if needed
# (Add version update logic here if you want to maintain version in files)

# Commit any changes
if [ -n "$(git status --porcelain)" ]; then
    print_info "Committing changes..."
    git add .
    git commit -m "Prepare release ${TAG}"
fi

# Create annotated tag
print_info "Creating tag ${TAG}..."
git tag -a "$TAG" -m "Release ${TAG}"

# Push changes and tag
print_info "Pushing to origin..."
git push origin main
git push origin "$TAG"

print_info "✓ Release ${TAG} created successfully!"
print_info ""
print_info "GitHub Actions will now:"
print_info "  1. Build the firmware"
print_info "  2. Create a GitHub release"
print_info "  3. Upload compiled binaries"
print_info ""
print_info "Monitor progress at:"
print_info "  https://github.com/mteinum/kobo-page-turner/actions"
print_info ""
print_info "Release will be available at:"
print_info "  https://github.com/mteinum/kobo-page-turner/releases/tag/${TAG}"
