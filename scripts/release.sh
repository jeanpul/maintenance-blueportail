#!/bin/bash

# config
VERSION=$(node --eval "console.log(require('./package.json').version);")
NAME=$(node --eval "console.log(require('./package.json').name);")

# build and test
npm test || exit 1

# checkout temp branch for release
git checkout -b gh-release

# commit changes with a versioned commit message
git commit -m "build $VERSION"

# push commit so it exists on GitHub when we run gh-release
git push origin gh-release

# create a ZIP archive of the dist files
(cd .. ; zip -r $NAME-v$VERSION.zip maintenance-blueportail)

# run gh-release to create the tag and push release to github
gh-release --assets $NAME-v$VERSION.zip

# checkout master and delete release branch locally and on GitHub
git checkout master
git branch -D gh-release
git push origin :gh-release


