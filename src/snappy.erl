%% snappy.erl -- wrapper of the Snappy algorithm
%%
%% This file is part of the erlang esnappyc data compression library.
%% Copyright (C) 2015 Oleksandr Chumachenko
%% All Rights Reserved.
%%
%% The esnappyc library is free software; you can redistribute it and/or
%% modify it under the terms of the GNU General Public License as
%% published by the Free Software Foundation; either version 2 of
%% the License, or (at your option) any later version.
%%
%% The esnappyc library is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%% GNU General Public License for more details.
%% You should have received a copy of the GNU General Public License
%% along with the esnappyc library; see the file COPYING.
%% If not, write to the Free Software Foundation, Inc.,
%% 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
%%
%% Oleksandr Chumachenko
%% <ledest@gmail.com>
%% https://github.com/Ledest/esnappyc/

-module(snappy).

-export([zip/1, unzip/1, compress/1, uncompress/1, decompress/1, validate/1, uncompressed_length/1]).

-on_load(load_nif/0).
-define(SNAPPY_NIF_VSN, 1).

load_nif() ->
    P = case code:priv_dir(snappy) of
            {error, bad_name} ->
                D1 = filename:join([".", "priv", "lib"]),
                case filelib:is_dir(D1) of
                    true -> D1;
                    _ ->
                        D2 = [$.|D1],
                        case filelib:is_dir(D2) of
                            true -> D2;
                            _ -> "."
                        end
                end;
            D -> D
        end,
    E = file:native_name_encoding(),
    L = filename:join(P, "snappy"),
    erlang:load_nif(L, {?SNAPPY_NIF_VSN, unicode:characters_to_binary(L, E, E)}).

-spec zip(Data::iodata()) -> binary().
zip(_Data) -> exit(snappy_nif_not_loaded).

-spec unzip(Data::iodata()) -> binary().
unzip(_Data) -> exit(snappy_nif_not_loaded).

-spec compress(Data::iodata()) -> binary().
compress(_Data) -> exit(snappy_nif_not_loaded).

-spec uncompress(Data::iodata()) -> binary().
uncompress(_Data) -> exit(snappy_nif_not_loaded).

-spec decompress(Data::iodata()) -> binary().
decompress(_Data) -> exit(snappy_nif_not_loaded).

-spec validate(Data::iodata()) -> boolean().
validate(_Data) -> exit(snappy_nif_not_loaded).

-spec uncompressed_length(Data::iodata()) -> integer().
uncompressed_length(_Data) -> exit(snappy_nif_not_loaded).
