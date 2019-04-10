# Kayak

This repository houses the bulk of the Kayak code.

## Contributing

As much as you would like to contribute, we want to allow you to contribute. This project is entirely student-driven, so we need as much help as we can get to improve it.

If you have ideas for improvements, even if it's not something you think you can tackle immediately, catalog it in the issue tracker. If you're looking for something to do, the issue tracker is a great place to pick up a bug. You should be able to assign yourself to issues if you find something to work on so we can avoid doubling our efforts.

### General procedure for contributing code:

1. Create a new branch from the tip of master. Commands to accomplish this:
   * `git checkout master`
   * `git pull`
   * `git checkout -b <branch-name>`
1. This branch is local to your machine, so no one else can see it yet. To register it with the remote repository:
   * `git push -u origin <branch-name>`
1. Create a template for your module
    * `MyGenMOOSApp <module-name> <prefix> "<your-name>"`
        * For example, to create a template for a module called iGPS, you would put `MyGenMOOSApp GPS i "Andrew Sucato"`
1. Do your feature development in the branch, committing and pushing along the way.
1. Open a merge request for the branch in GitLab.
   * Sometimes there will be a handy button on the front page to let you do this — other times, just go to the Branches page and click the Merge Request button for your branch.
   * Be sure to add a written desciption of your change: include rationale, tag associated issues, and highlight major design decisions.
1. Tag some people in comments to review your Merge Request.
   * Make sure to tag at least one of the software leads, since they'll eventually perform the merge to master once the feature is finished.
   * Peer reviews are cool too — if you worked closely with someone, or if you are changing a part of code you know someone else is working on, get them in on the action.
1. Finally, tend the merge request until it's closed.
   * Chances are high that code reviewers will find something to tweak before merging. This isn't a slight on your coding skill — it's the purpose of review, and it helps everyone to improve.
   * Faster response times during merge requests decrease time between initial request and final merge.

### Branching guidelines:

 * The branch name should describe the feature it's concerned about. Good examples: `merge-lidar-utilities`, `remove-ahrs`, `fix-gimbal-balancing`
 * Develop only one feature per branch. Focused changes = faster merges.
 * Always create new branches from the tip of master.
 * Never start a new branch from another feature branch.
 * If there are small issues you find that aren't related to the feature at hand, don't fix them in that branch. Keep changes isolated.

