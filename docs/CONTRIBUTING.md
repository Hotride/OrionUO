# Contributing to OrionUO

This document is split up and organised into several sections to aid reading and linking. For small changes like one-line fixes or minor tweaks then you can just read the [quick start section](#quick-start) below.

Don't worry about reading all of these documents end-to-end and getting everything perfect the first time. The point of this information isn't to be restrictive about rules and reject contributions, but to give people guidance and help about how to contribute.

If you're a regular contributor or if you have a larger amount of code to change, please do read through these as it will make life easier for everyone if you to follow along with these guidelines from the start.

## Code of Conduct

We want to ensure that anyone can contribute to OrionUO with only the next bug to worry about. For that reason the project has adopted the [contributor covenent](CODE_OF_CONDUCT.md) as a code of conduct to be enforced for anyone taking part in OrionUO development. This includes any comments on issues or any public discussion e.g. in the OrionUO discord channel.

## Copyright / Contributor License Agreement

Any code you submit will become part of the repository and be distributed under the [OrionUO license](../LICENSE). By submitting code to the project you agree that the code is your own work and that you have the ability to give it to the project.

You also agree by submitting your code that you grant all transferrable rights to the code to the project maintainers, including for example re-licensing the code, modifying the code, distributing in source or binary forms. Specifically this includes a requirement that you assign copyright to the project maintainers.

## Contributing information

1. [Compiling](CONTRIBUTING/Compiling.md)
2. [Preparing commits](CONTRIBUTING/Preparing-Commits.md)
3. [Developing a change](CONTRIBUTING/Developing-Change.md)
4. [Testing](CONTRIBUTING/Testing.md)

## Quick Start

The two things you'll need to bear in mind for a small change are the [commit message](CONTRIBUTING/Preparing-Commits.md#commit-messages) and [code formatting](CONTRIBUTING/Preparing-Commits.md#code-formatting).

Commit messages should have a first line with a **maximum of 72 characters**, then a gap, then if you need it a longer explanation in any format you want. The reason for this is that limiting the first line to 72 characters means that `git log` and github's history always displays the full message without it being truncated.

For more information, check the section about [commit messages](CONTRIBUTING/Preparing-Commits.md#commit-messages).

Code should be formatted using **clang-format 3.8**. The reason we fix a specific version of clang-format is that unfortunately different versions can format code in different ways using the same config file, so this would cause problems with automatic verification of code formatting.

For more information, check the section about [code formatting](CONTRIBUTING/Preparing-Commits.md#code-formatting).

## Where to Start

There are always plenty of things to do, check out the [planning](https://github.com/orgs/OrionUO/projects) page for tasks to tackle, or have a look at the [issues](https://github.com/OrionUO/OrionUO/issues) for outstanding bugs. We'll try and tag things that seem like small changes that would be a good way for someone to get started with.

If you have a change you'd like to see make it into `master`, create a fork, make your changes to a branch, and open a pull request on github. You can look around for instructions on that - it's pretty simple.

Have a clear idea of what your change is to do. This goes hand in hand with the above, but if your change involves a lot of work then it's better to split it up into smaller components that can be developed and merged individually, towards the larger goal. Doing this makes it more easily digestible for the rest of the people on the project as well as making it easier to review the changes when they land.
