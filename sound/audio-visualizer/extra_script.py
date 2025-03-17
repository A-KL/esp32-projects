import os

Import("env")

env.BuildSources(
    os.path.join("$BUILD_DIR", "test"),
    os.path.join("$PROJECT_DIR", "test")
)