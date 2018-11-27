# Facebook Games SDK
The Facebook Games SDK is an open source C++ framework for integrating
Facebook's Login and Graph API functionality into your Windows game.

It handles:
  * Accessing the login dialog, either through the system's default browser
  or via another implementation
  * Handling the OAuth response and access token via a local listener
  (server-side implementation is suggested)
  * Making Graph API calls such as getting a user's friend list.
  * Sending app events

## Examples
The project includes a sample demo app designed to demonstrate some of the
functionality.

## Requirements
Facebook Games SDK requires or works with
* Windows 7 or later

## Building the Facebook Games SDK
The SDK is supplied as a header-only library and only needs to be #included

The Game SDK depends on [Microsoft's C++ REST SDK ("Casablanca")](https://github.com/Microsoft/cpprestsdk).

## Getting started
The SDK is designed to be modular and flexible.  
Dialog-based calls such as Facebook Login require a game-accessible browser.  
Two such browsers are currently available;
others can be easily added by deriving from IBrowser.

Although by default the login redirect will be handled locally, it is better
to redirect to an address on your server that can serve a dedicated landing
page, and to store the user access token for server-side API calls.

## License
The Facebook Games SDK is offered under the Facebook Platform license,
as found in the LICENSE file.
