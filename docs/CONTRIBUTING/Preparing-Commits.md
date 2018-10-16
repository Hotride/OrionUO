# Preparing commits

## Commit Messages

Ensure commit messages have a single title of at most 72 characters, followed by a body if necessary:

```
Short summary text, maximum of 72 characters. That's out to here ---->X

Longer paragraph that details the change and any logic behind needing
to make the change, other impacts, future work etc. Keeping the title
to 72 characters means it will display fully in git log and github logs.

Any change worth of a release notes mention, please include a one line
description starting with a `-`. Example:

- Now supporting this awesome feature.
```

Merge commits should **not** be included in pull requests as they just muddy the history. Please use `git rebase` when bringing code up to date against latest `master`.

Likewise commits for code formatting or compile fixes should be squashed into the relevant commits that they update, rather than left in the history.

## Code formatting

To make things easier for everyone, we've adopted clang-format for keeping code consistently formatted.

| :warning: |                                                                                                                                         | :warning: |
| --------- | --------------------------------------------------------------------------------------------------------------------------------------- | --------- |
| :warning: | **Unfortunately clang-format output changes between versions even with the same config.**                                               | :warning: |
| :warning: | **We have fixed the version used for OrionUO at clang-format-7.0.**                     | :warning: |
| :warning: | **This formatting is enforced by CI checks that run on PRs, so if you aren't running the same version locally it will show up there.**  | :warning: |

If you want to format a single file, run: `clang-format -i file.cpp`.

On Visual Studio 2017 or later, clang-format is already integrated, please setup it accordingly. Go to `Tools > Options > LLVM/Clang > Enable = True`, and everything should be fine. We'll use clang-format 7.0 by default as this is the version that comes with Visual Studio 2017.

All intermediate commits should follow the formatting conventions. Having several intermediate commits with mismatched formatting then a single 'reformatted code' commit at the end makes history harder to read, which is an important tool for others to understand your code. It is much easier to enforce proper formatting on each commit as you go along, than to try and untangle and merge formatting changes in after the fact.

## Branch history

Different people have different preferences on how history should be organised. Some people prefer to commit often and then squash down changes into a single commit after they've finished working. Others prefer to combine commits intermittently when they reach a logical boundary and or next step. Some like to avoid committing until they have a 'final' commit that they are ready to make.

We're flexible about whether branches are squashed or expanded when they are put up for PR. The history should be reasonable though; if you have a change that modifies a dozen files and changes hundreds of lines of code then it should not be squashed into a single commit. Likewise, intermittent changes where something was tried and then reverted, or minor fixes and tweaks as you go should be rebased and squashed together to form a more coherent whole. As a rule of thumb, try to keep your commit messages describing what they do roughly, even if it is a minor change. Commit messages like "fix stuff" or "compile fix" or any message with "WIP", "amend", "temp", etc should probably not remain in the final PR.

With fixes from a code review it's up to you whether you keep changes as a separate commit or squash them in. With the exception of formatting and compile fix commits, which as noted before should be squashed into the relevant commits.

For overall scope of a change/branch read the [developing a change](Developing-Change.md) section.

