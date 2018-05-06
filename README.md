# JigsawPuzzle

Solution to challenge https://www.reddit.com/r/dailyprogrammer/comments/8fbqhw/20180427_challenge_358_hard_puzzle_me_this/.

## Solver: jigsaw_puzzle

Reads the puzzle data on standard input. Brute force backtracker that locks the up/left corner with the first tile that fits and performs a row by row scan. Does a full search, the first solution found is displayed and the total number of solutions is provided at the end of execution.

## Generator: jigsaw_puzzle_gen

Reads 5 parameters on standard input:
- number of columns
- number of rows
- number of possible border edges
- number of possible inner edges
- rotate tiles flag

Generates a random puzzle that conforms with the parameters provided in a format that can be used by the solver.

## Puzzle data format
`
\<number of columns\>, \<number of rows\>, \<rotate tiles flag\>
\<tile id\>: \<up edge\>, \<right edge\>, \<down edge\>, \<left edge\>
...
\<tile id\>: \<up edge\> \<right edge\> \<down edge\> \<left edge\>

The "rotate tiles flag" indicates whether the tile edges were randomly rotated or not.
`
A tile can connect with tiles that have an opposite edge value.
