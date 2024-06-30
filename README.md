# 2024-group-06 Automotive


## Cloning

```
cd project-name
git clone git@git.chalmers.se:courses/dit638/students/2024-group-06.git
```

## Toold & build
You need to open the terminal and install the following programs with these commands:

```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install build-essential cmake git
```

Make sure that you have docker, Cmake, docker-compose, g++, make & git installed

## Integrate with your tools

- [ ] [Set up project integrations](https://git.chalmers.se/courses/dit638/students/2024-group-06/-/settings/integrations)

## Collaborate with your team

- [ ] [Invite team members and collaborators](https://docs.gitlab.com/ee/user/project/members/)
- [ ] [Create a new merge request](https://docs.gitlab.com/ee/user/project/merge_requests/creating_merge_requests.html)
- [ ] [Automatically close issues from merge requests](https://docs.gitlab.com/ee/user/project/issues/managing_issues.html#closing-issues-automatically)
- [ ] [Enable merge request approvals](https://docs.gitlab.com/ee/user/project/merge_requests/approvals/)
- [ ] [Set auto-merge](https://docs.gitlab.com/ee/user/project/merge_requests/merge_when_pipeline_succeeds.html)

## Test and Deploy

Use the built-in continuous integration in GitLab.

- [ ] [Get started with GitLab CI/CD](https://docs.gitlab.com/ee/ci/quick_start/index.html)
- [ ] [Analyze your code for known vulnerabilities with Static Application Security Testing (SAST)](https://docs.gitlab.com/ee/user/application_security/sast/)
- [ ] [Deploy to Kubernetes, Amazon EC2, or Amazon ECS using Auto Deploy](https://docs.gitlab.com/ee/topics/autodevops/requirements.html)
- [ ] [Use pull-based deployments for improved Kubernetes management](https://docs.gitlab.com/ee/user/clusters/agent/)
- [ ] [Set up protected environments](https://docs.gitlab.com/ee/ci/environments/protected_environments.html)

***

## Description
This project is about developing an algorithm to train a miniture car with various sensors to be able to navigate and self drive in a track race. 

## Visuals
Depending on what you are making, it can be a good idea to include screenshots or even a video (you'll frequently see GIFs rather than actual videos). Tools like ttygif can help, but check out Asciinema for a more sophisticated method.

## Installation
Within a particular ecosystem, there may be a common way of installing things, such as using Yarn, NuGet, or Homebrew. However, consider the possibility that whoever is reading your README is a novice and would like more guidance. Listing specific steps helps remove ambiguity and gets people to using your project as quickly as possible. If it only runs in a specific context like a particular programming language version or operating system or has dependencies that have to be installed manually, also add a Requirements subsection.

## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Authors and acknowledgment
Owner: 
- P. Christian Berger @christian.berger

Members: 
- Nawras Kanjo @nawrask
- Ahmad Haj Ahmad @haja
- Josef Almasri @josefab
- Rizwan Rafiq @rizwanra

## License
[License](LICENSE)

## Issue Management
- Creating Issues: Every task or feature starts with creating an issue in Chalmers GitLab. This issue should clearly describe the task, including goals, any necessary background information, and potential steps for completion.

- Assignment: Once an issue is created, it is assigned to a specific developer. This ensures clear responsibility for tasks and avoids duplication of effort.

 ## Branching Strategy
- One Branch per Issue: For each issue, a single branch is created. This branch is named in a way that reflects the issue it addresses, ensuring a direct correlation between work done and the corresponding issue.

- No Parallel Work on the Same Branch: To maintain focus and avoid conflicts, only the assigned developer works on the branch related to an issue. If collaboration or help is needed, it is done through a code-help session, not by having multiple people commit to the same branch. 

## Merge Requests and Code Reviews
- Creating Merge Requests: Upon completion of work on a branch, the developer creates a merge request (MR). The MR is assigned to another team member for review. This process ensures that all code is reviewed by at least one other team member before it is merged.

- Pipeline Passing Requirement: Before a branch can be merged into the main branch, it must pass all defined pipelines. This ensures that new code does not break the build and adheres to defined quality standards.

- Merging: Any team member is allowed to merge a branch into the main branch once it has passed the pipelines and received approval through the MR process. This policy fosters shared responsibility for the codebase.

## Handling Merge Conflicts
- Responsibility for Merge Conflicts: In cases where multiple branches are being worked on simultaneously, it is possible that merges into the main branch may create conflicts in other branches. It is the responsibility of the developer working on the unmerged branch to pull changes from the main branch, resolve any merge conflicts, and then proceed with creating a merge request for their branch.

## Force Pushes and Branch Deletion
- Restrictions: Force pushing to branches and deleting branches are strictly prohibited. These actions can disrupt the workflow and potentially cause work to be lost. All changes should be made through the standard commit process and merge requests.

## Incremental and Atomic Commits
What to Do:
- Commit Small Changes: Regularly commit small, self-contained changes.  This makes your work clearer and easier to review.
- Keep It Logical: Each commit should represent a single logical change. This helps in understanding the change and pinpointing issues.
- Clear Messages: Write clear commit messages that explain what you did and why, adhering to our project's guidelines.

# Code Review Guidelines
## Purpose
The purpose of code review is to improve the quality of the code by identifiyng and correcting defects, improving maintainability, and ensuring compliance with coding standards.

## Goals
- Find defects early in the development cycle
- Improve code maintainability
- Ensure code complies with coding standards
- Share knowledge and promote team communication


## Pre-Review
Before starting a code review, ensure that:
- The code is complete and tested
- The code follows already established coding standards
- The code is documented (as appropriate)
- The changes are clear and easy to understand
- The author has provided sufficient context for the changes

## During the Review
During the code review, keep the following in mind:
- Stay focused on the code and avoid personal criticism
- Identify specific issues and provide constructive feedback
- Consider both the short-term and long-term impact of the changes
- Verify that the changes are necesary and appropriate
- Ensure that the code is maintainable and extensible
- Discuss design decisions and alternatives

## Post-Review
After the code review, the author should:

- Address all feedback and defects identified during the review
- Provide an updated version of the code for review, if necessary
- Document any significant changes made in response to the review

# General rules for commits
1. The commits should be small and self-contained (atomic), see more info [here](https://www.freshconsulting.com/insights/blog/atomic-commits/)
2. Only commit when an issue/block of work is **complete**.
3. Commit each fix/task as separate changes

## What a commit should look like

### 1. Separate subject from body with a blank line ###

When making a commit in Git, it's a good practice to start with a brief summary of the change, followed by a more detailed description. The first line of the commit message is treated as the commit title and is used throughout Git, including in email subjects. However, for simple changes, a single line summary may be sufficient and a more detailed description may not be necessary.

### 2. Limit the subject line to 50 characters ###

Limiting the subject line of a commit message to around 50 characters is a recommended practice to promote readability and encourage the author to succinctly convey the purpose of the change.

### 3. Capitalize the subject line ###

Begin all subject lines with a capital letter.

### 4. Do not end the subject line with a period ###

Implementing punctuation is unnecessary in short subject lines. Space is precious, so it's best to use it wisely.

### 5. Use the imperative mood in the subject line ###

Implement exclusively the imperative mood. Think of the following statement "If applied, this commit will" before writing the subject line.

### 6. Wrap the body at 72 characters ###

Each line in the body of the commit message should not exceed 72 characters.

### 7. Use the body to explain _what_ and _why_ vs. _how_ ###

When writing a commit message, focus on explaining the reason for the change, rather than describing the technical details of the code. Use comments within the code to explain complex logic or implementation details. By providing clear explanations in the commit message, you can make it easier for future maintainers (including yourself) to understand the intent behind the change.

For additional details, please refer to the following link: https://cbea.ms/git-commit/

## Project status
Under development.
