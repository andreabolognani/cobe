-module(polish).
-author("Giovanni [dacav] Simoni").
-export([rpc/1]).

get_operator (Op) ->
    OpCls = [{'+', fun(X, Y) -> X + Y end},
             {'-', fun(X, Y) -> X - Y end},
             {'*', fun(X, Y) -> X * Y end},
             {'/', fun(X, Y) -> X / Y end}],
    case [F || {O, F} <- OpCls, O == Op] of
        [] -> throw('not an operator');
        [F] -> F
    end.

rpc_apply (Stack, Tok) when is_number(Tok) -> [Tok | Stack];
rpc_apply (Stack, Tok) ->
    try
        [ X, Y | Rem ] = Stack,
        F = get_operator(Tok),
        [ F(X,Y) | Rem ]
    catch
        error:Err -> 
            io:format("Fuck: Stack=~p Tok=~p", [Stack, Tok]),
            throw({'invalid execution', Err})
    end.

rpc (Seq) -> 
    [Res] = lists:foldl(fun (Stack, Tok) -> rpc_apply(Tok, Stack) end,
                        [], Seq),
    Res.

