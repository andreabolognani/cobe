-module(test).
-compile(export_all).

listen_and_print () ->
    receive
        X -> io:format("I've got '~p'~n", [X])
    end,
    listen_and_print().

