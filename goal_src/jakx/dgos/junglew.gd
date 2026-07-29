;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; Dropped code objects, MIPS code that cannot be raw-copied (their top-levels would
;; execute at link); only jungle-obs.o remains dropped, and it lands with the rest of
;; the jungle actor-code leg. jungle-part.o and jungle-part2.o compile from goal_src
;; again, and code objects must link ahead of the art and bsp data. The effects pair
;; and jungle-ocean landed with the water leg (pure data).
("JUNGLEW.DGO"
 ("jungle-part.o"
  "jungle-part2.o"
  "jungle-effects.o"
  "havjung-effects.o"
  "jungle-ocean.o"
  "tpage-544.go"
  "jungle-flaming-arrow-ag.go"
  "jungle-branch-collision-a-ag.go"
  "junglew.go"
 ))
