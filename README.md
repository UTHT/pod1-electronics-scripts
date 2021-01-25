# Electronics Scripts

Libraries and test sketches for the various electronics (sensors, etc.).


# Development

> New to Git? Watch [this tutorial](https://youtu.be/h8c2dznNWL8) first.

> Never heard of "SemVer"? Look [here](https://semver.org/).

## Repo Structure

Refer to the `examplesensor/` folder.

Folders will exist, each containing: 
- An Arduino "test sketch" named after the sensor (i.e. "`VN200.ino`"), used to test the sensor *individually*;
- The library ("`VN200.cpp`" & "`VN200.h`") that can be imported by the "main" Arduino sketch*;
- Any dependancy libraries will be nested inside a sub-folder named "libraries". 

> \*It can be expected that the test sketch will precede the library in development, and will initially contain the library code (how to access the sensor's data) along with testing procedure (executing those access/communication functions).

In addition, a `README.md` Markdown document will accompany each sensor's sketch, detailing:

- The development requirements/dependencies for this sketch/library;
- The testing and execution steps;
- Expected output;
- Other documentation (i.e. pinout diagrams, schematics, expected output) or references to where they can be found.

## Feature Branches

One feature branch will exist for *each* sensor (i.e. "`sensor-vn200`" for the VN200), on which all changes for that sensor's code will be committed. Changes to other files *should* be avoided.

### Pull Requests (w/ Testing Documentation)

Feature branches are to be pulled into a **staging** branch, and pull requests **MUST**:
- Have the team leaders (i.e. Orest, Urmil) listed as "Reviewers" so that they can review the PR;
- List a summary of changes made in the feature branch commits leading up to the PR;
- Provide documentation proving working functionality (i.e. whoever is in possession of the sensor corresponding to the feature branch **MUST** be able to *clone*, *setup*, and *run* the code successfully - without modification - meeting the *expected output*);

Upon confirmation of successful operation, the PR will be **squash merged** by one of the team leaders as a SemVer-compliant versioned change (i.e. "VN200 Test Sketch V0.1" for an early beta sketch, or "VN200 Library V1.0" for a well-tested library full release).

### Production Releases

When all sensor code is a working library release (i.e. can be imported by a single "main" sketch and run, with sub-dependancy libraries in one master "libraries" folder), then the staging branch will be *pulled* and *squash-merged* to master by the Team Leaders, and a SemVer-compliant GitHub release tag will be added.