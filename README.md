# PyOpenNI, round II


Yep, you guessed it, these are the fantastic, incredible **[OpenNI2][OpenNI2]
bindings for the [Python][Python] scripting language**.

This is a follow-up to [PyOpenNI][PyOpenNI], which binded the (now legacy)
[OpenNI1][OpenNI]; this has been rewritten from scratch and uses Python's
recommended way af building, [Distutils][distutils], instead of [CMake][CMake].
This makes PyOpenNI2 *really* easy to install, see [Installing][#installing].

[Boost::Python][BoostPython] *is the way to go* with OpenNI2, which is a
C++ API (in contrast to OpenNI1, which was a C API). 99% of the wrapper is
contained in a single file, `wrapper.cc`.

PyOpenNI2 is made available under a [2-clause BSD license]
(http://opensource.org/licenses/BSD-2-Clause), see `LICENSE`.  
Feedback? Problems? Want a feature? Did I chose an original name?  
[File an issue](https://github.com/jmendeth/PyOpenNI2/issues/new)
or pull-request.


## Installing

You'll need:

 * **Python** (of course) and its **development files**.
 * **OpenNI2** (the latest stable release should work). You can download
   ready-to-use binaries at [the website](http://www.openni.org/openni-sdk).
   *Don't forget to run `sudo ./install.sh`!*
 * **Boost::Python**, and its development files.
   If it's in a non-standard location, you may need to tweak `setup.py`.

Now, just run the usual command:

```bash
$ python setup.py install
```

You may need to run it with `sudo -E`.
For more information about installing Python modules, look
[here](http://docs.python.org/2/install/#standard-build-and-install).

#### I have a problem in Windows that...

**No.** Windows is like the *different one*, and it's intentional. Every other
system follows the standards. I don't care supporting it (well, others may)
because Microsoft hasn't made a single action to support me as a developer.

After that mini-rant, left to say is that you can search for people
having the same problem, install MinGW, switch to another OS (it's
worth the change) or file an issue. But in the latter, *don't expect
much support*. What's more, the whole `distutils` doesn't support it.


## Usage

Once it's installed, just import it:

```python
import openni2
```

Some documentation is available via docstrings, viewable with Python's own
help system, but there's still a lot missing.

```python
help(openni2)
help(openni2.Device)
```

Also take a look at the `examples` directory.




[Python]: http://python.org "Python Language website"

  [OpenNI]:  https://github.com/OpenNI/OpenNI      "OpenNI1 repository"
  [OpenNI2]: https://github.com/OpenNI/OpenNI2     "OpenNI2 repository"
[PyOpenNI]:  https://github.com/jmendeth/PyOpenNI  "PyOpenNI repository"
[PyOpenNI2]: https://github.com/jmendeth/PyOpenNI2 "PyOpenNI2 repository"

[c_api]: http://docs.python.org/2/c-api "Python/C API documentation"
[BoostPython]: http://boost.org/libs/python "Boost::Python documentation"

[distutils]: http://docs.python.org/2/extending/building.html "Building extensions with Distutils"
[CMake]: http://cmake.org "CMake website"
