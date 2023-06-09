#!/bin/env julia

using DataFrames, CSV, CairoMakie, Statistics

df_fp1 = CSV.read("./fpga_final_solve1.csv", DataFrame)
df_fp1_fix = combine(
    groupby(df_fp1, :puzzle_size),
    :time_one_ms => mean;
    renamecols=false,
)

df_fp8 = CSV.read("./fpga_final_solve8.csv", DataFrame)
df_fp8_fix = combine(
    groupby(df_fp8, :puzzle_size),
    :time_total_ms => mean;
    renamecols=false,
)

fpga_f = Figure()
fpga_ax = Axis(
    fpga_f[1, 1],
    title="Zybo ARM Chip Solve Performance",
    xlabel="Puzzle Size",
    ylabel="Solve Time (ms)",
    yscale=log10,
)
lines!(fpga_ax, df_fp1_fix[!, :puzzle_size], df_fp1_fix[!, :time_one_ms], label = "single solution")
lines!(fpga_ax, df_fp8_fix[!, :puzzle_size], df_fp8_fix[!, :time_total_ms], label = "up to 8 solutions")
fpga_legend = Legend(fpga_f[1, 2], fpga_ax)
save("../images/fpga_perf.png", fpga_f)

local_solves = CSV.read("./local_solves.csv", DataFrame)
local_solves_fix = transform(
    local_solves,
    :time_s => (n -> n * 1000) => :time_ms
)

local_solves1 = filter(:single => identity, local_solves_fix)
local_solves8 = filter(:single => (b -> !b), local_solves_fix)

host_f = Figure()
host_ax = Axis(
    host_f[1, 1],
    title="Local Machine Solve Performance",
    xlabel="Puzzle Size",
    ylabel="Solve Time (ms)",
    yscale=log10,
)
lines!(host_ax, local_solves1[!, :size], local_solves1[!, :time_ms], label="single solution")
lines!(host_ax, local_solves8[!, :size], local_solves8[!, :time_ms], label="up to 8 solutions")
host_legend = Legend(host_f[1, 2], host_ax)
save("../images/host_perf.png", host_f)
