
upSL (U<font color="#aaa">rsine</font>P<font color="#aaa">aw's </font>S<font color="#aaa">core</font>L<font color="#aaa">oop wrapper</font>)
===============

## What is upSL?
upSL is a simple, cross-platform wrapper for Scoreloop functions in game developed with [cocos2d-x][1]. It is works on ios, android and blackberry(playbook).

Main idea is that all functions and API must be same for all supported platforms, so if some function realised only for blackberry we haven't it in upSL.

It is provide C-style API. Never store pointers to objects that created by Scoreloop, except controllers, of course, since it created directly by you.

We don't cover all parts of Scoreloop, only things what we need in our games. Feel free make code modifications and share it with us :)

---
## Integration guide

We use it with:

* Scoreloop 4.3 for iOS
* Scoreloop 1.0 for BlackBerry (included in blackberry native sdk 2.0)
* Scoreloop 3.0.2 for Android

### Initialize and shutdown
You should add:

* <code>Scoreloop::Client::Initialize(TOSAccepted);</code> in <code>Application::applicationDidFinishLaunching()</code>.</br>
Where <code>TOSAccepted</code> is a callback which called when user accept TOS or immediatly after initialisation if he did it early

* <code>Scoreloop::Client::Shutdown();</code> in Application::~Application()

<b>!NOTICE!</b>
You can use scoreloop functions only if receive TOSAccepted callback.

### Platform specific part
#### iOS
Substitute value of "game id" and "secret key" in ScoreloopManager.mm
<pre><code>#define SL_GAME_ID "YOUR GAME ID"
#define SL_SECRET_KEY "YOUR GAME SECRET KEY"</code></pre>

#### Android

Add in <code>onCreate()</code> method of Application class <pre><code>Scoreloop.init(this, "YOUR GAME SECRET KEY");</code></pre>
Add in <code>onCreate(Bundle savedInstanceState)</code> method of Activity class <pre><code>Scoreloop.setActivity(this);
Scoreloop.setGLSurfaceView(mGLView);</code></pre> 

#### Blackberry
You should have 
<pre><code>if (Scoreloop::HandleBSPEvent(event))
return true;</code></pre>
in your event handler, see <code>void CCEGLView::setEventHandler(EventHandler* pHandler)</code> which introduced in [cocos2d-x][1] with [our pull request][2]

Don't forget to substitute value of "game id" and "secret key" in qnx_scoreloop.cpp
<pre><code>#define SL_GAME_ID "YOUR GAME ID"
#define SL_SECRET_KEY "YOUR GAME SECRET KEY"</code></pre>

---

## License
### Summary

upSL (the software) is licensed under the terms of the MIT license.

### MIT License

Copyright &copy; 2012 Ursine Paw.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[1]: https://github.com/cocos2d/cocos2d-x
[2]: https://github.com/cocos2d/cocos2d-x/pull/730
