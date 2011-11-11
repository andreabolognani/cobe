-module(spamserver).
-export([start/0, insert/1, remove/1]).

-behavior(gen_server).
-export([init/1, code_change/3, handle_call/3, handle_cast/2,
         handle_info/2, terminate/2]).

-define(TIMEOUT, 1000).
-record(state, {targets=[],
                timeout=infinity}).

add_target (State, T) ->
    #state{targets=[T | State#state.targets],
           timeout=?TIMEOUT}.

rem_target (State, T) ->
    Ts = lists:filter( fun (X) -> T =:= X end, State#state.targets ),
    To = case Ts of
         [] -> 
            io:format(standard_error, "No targets! Stopping spam~n", []),
            infinity;
         _ ->
            io:format(standard_error, "Got targets! Starting spam~n", []),
            ?TIMEOUT
         end,
    #state{targets=[T | Ts], timeout=To}.

init (_) -> {ok, #state{}, infinity}.

spam_targets (State = #state{}) ->
    case State#state.targets of
    [] -> io:format(standard_error, "Something nasty here!~n", []);
    Ts -> lists:foreach(fun (P) -> P ! spam end, Ts)
    end.

code_change (_, State, _) -> {ok, State}.

handle_call ({start, T}, From, State) ->
    Response = case From of
               {T,_} -> fool;
               _ -> ok
               end,
    NewState = add_target(State, T),
    {reply, Response, NewState, NewState#state.timeout};

handle_call ({stop, T}, _, State) ->
    NewState = rem_target(State, T),
    {reply, ok, NewState, NewState#state.timeout};

handle_call (report, {From, Ref}, State) ->
    From ! {Ref, State#state.targets},
    {reply, check_mail, State}.

handle_cast (_, State) ->
    {noreply, State}.

handle_info (Info, State) ->
    case Info of
        timeout -> spam_targets(State);
        X -> io:format(standard_error, "Got ~p, don't get that…~n", [X])
    end,
    {noreply, State, State#state.timeout}.

terminate (Reason, State) ->
    io:format(standard_error, "Terminating because of ~p. State: ~p~n",
              [Reason, State]),
    ok.

start () ->
    gen_server:start({local, ?MODULE}, ?MODULE, none, []).

insert (P) ->
    gen_server:call(?MODULE, {start, P}).

remove (P) ->
    gen_server:call(?MODULE, {stop, P}).

