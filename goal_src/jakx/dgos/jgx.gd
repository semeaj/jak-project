;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; Dropped code objects, MIPS code that cannot be raw-copied (their top-levels would
;; execute at link); only jungle-obs.o remains dropped, and it lands with the rest of
;; the jungle actor-code leg. jungle-part.o and jungle-part2.o compile from goal_src
;; again, and code objects must link ahead of the art and bsp data. The effects pair
;; and jungle-ocean landed with the water leg (pure data).
("JGX.DGO"
 ("jungle-part.o"
  "jungle-part2.o"
  "jungle-effects.o"
  "havjung-effects.o"
  "jungle-ocean.o"
  "tpage-1601.go"
  "tpage-1603.go"
  "tpage-1600.go"
  "jungle-debris-jar-a-ag.go"
  "jungle-debris-jar-b-ag.go"
  "jungle-debris-ag.go"
  "jungle-clay-jar-b-ag.go"
  "jungle-clay-jar-a-ag.go"
  "jungle-tree-root-large-b-ag.go"
  "jungle-tree-root-large-a-ag.go"
  "jungle-flaming-arrow-ag.go"
  "junglex-vis.go"
 ))
