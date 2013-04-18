# Contribution guidelines

First of all, the project philosophy:

- We prefer to sacrifice speed a bit (Boost::Python is slow
  already) for simplicity.
- The API should be understandable, modular, structured and easy to use.
- Implementation and low-level features (like the exception wrapping)
  should be kept transparent to the user. Don't explain them in docstrings
  or other user-facing documentation.
- Examples are an excellent way to learn.

## What should I do..?

#### Before contributing

- Read *all* of those guidelines. :)

#### When reporting issues

- Make sure you have that issue **with the `unstable` branch code**.
- Always search for similar issues to avoid duplicates.
- You should look at the documentation of the corresponding method or class,  
  in case it's an expected bug and/or a workaround is available.
- If you're going to report an issue, please provide
  some info about your environment, like:

  * Platform
  * Python version
  * OpenNI2 version
  * Boost::Python version

#### When developing

- Always, **always work on the `unstable` branch**.  
  If not, you're eligible for being shot.
- We use **two-space indentation for the C++ code.
- Don't **overpass the 80-char margin** very often.
- Use the "funny style" inside function parameters, etc.
- Make the code **understandable and expressive**; add
  comments where necessary.

#### When submitting Pull Requests

- I think it's clear already, but just in case:
  **Do it in the `unstable` branch.**
- Be sure to follow the developing and philosophy guidelines listed above.
- Try not to add new files, except if they're examples.
- Don't add code which **depends too much on the environment**.
- Implemented a new feature? **Add the adequate example** where possible.
