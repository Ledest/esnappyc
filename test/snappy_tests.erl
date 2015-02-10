%% snappy_test.erl -- unit test for wrapper of the esnappyc library
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

-module(snappy_tests).
-include_lib("eunit/include/eunit.hrl").

snappy_test_() ->
    {timeout, 60, [fun snappy/0]}.

snappy() ->
    {ok, Data} = file:read_file("../README.md"),
    Compressed1 = snappy:zip(Data),
    Decompressed1 = snappy:unzip(Compressed1),
    {ok, Compressed2} = snappy:compress(Data),
    {ok, Decompressed2} = snappy:decompress(Compressed2),
    {ok, Decompressed3} = snappy:uncompress(Compressed2),
    ?assertEqual(Data, Decompressed1),
    ?assertEqual(Data, Decompressed2),
    ?assertEqual(Data, Decompressed3).
