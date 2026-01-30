# Git + GitHub Cheat Sheet

Quick reference for Git and GitHub. All commands run from the project folder unless noted.

---

## First time (new repo)

```bash
git init
git add .
git commit -m "Initial: your message"
git branch -M main
git remote add origin https://github.com/USER/REPO.git
git push -u origin main
```

**If `remote origin already exists`** — skip `git remote add`. To change URL: `git remote set-url origin <new-url>`.

**If `src refspec main does not match any`** — you have no commits yet. Run `git add .` and `git commit -m "..."` first, then push.

---

## Clone (get a repo)

```bash
git clone https://github.com/USER/REPO.git
cd REPO
```

---

## Daily flow

| Do this             | Command                             |
| ------------------- | ----------------------------------- |
| See what changed    | `git status`                        |
| See diff (unstaged) | `git diff`                          |
| Stage all           | `git add .`                         |
| Stage one file      | `git add path/to/file`              |
| Unstage a file      | `git restore --staged path/to/file` |
| Commit              | `git commit -m "Your message"`      |
| Push                | `git push`                          |
| Pull (get updates)  | `git pull`                          |

---

## Branches

| Do this               | Command                                                        |
| --------------------- | -------------------------------------------------------------- |
| List branches         | `git branch`                                                   |
| Create branch         | `git branch feature-name`                                      |
| Switch branch         | `git checkout feature-name` or `git switch feature-name`       |
| Create and switch     | `git checkout -b feature-name` or `git switch -c feature-name` |
| Push branch to remote | `git push -u origin feature-name`                              |

---

## Undo / fix

| Do this                                         | Command                                                      |
| ----------------------------------------------- | ------------------------------------------------------------ |
| Discard changes in file (unstaged)              | `git restore path/to/file` or `git checkout -- path/to/file` |
| Unstage file (keep changes)                     | `git restore --staged path/to/file`                          |
| Amend last commit (change message or add files) | `git add .` then `git commit --amend -m "New message"`       |
| See commit history                              | `git log` or `git log --oneline`                             |

---

## Remote

| Do this           | Command                           |
| ----------------- | --------------------------------- |
| List remotes      | `git remote -v`                   |
| Add remote        | `git remote add origin <url>`     |
| Change remote URL | `git remote set-url origin <url>` |
| Remove remote     | `git remote remove origin`        |

---

## GitHub (on the web)

- **New repo**: GitHub → **+** → **New repository** → name it → **Create**. Don’t init with README if you already have a local repo.
- **Repo URL**: `https://github.com/USER/REPO.git` (HTTPS) or `git@github.com:USER/REPO.git` (SSH).
- **Fork**: Copy someone’s repo to your account. **Clone** your fork, then add **upstream**: `git remote add upstream https://github.com/ORIGINAL-USER/REPO.git`. Pull upstream: `git pull upstream main`.

---

## One-liner flow

```bash
git add .
git commit -m "Describe what you did"
git push
```

---

## Related docs

- [SETUP.md](SETUP.md) — Build, run, compilers, troubleshooting.
- [INDEX.md](INDEX.md) — Doc map and learning path.
