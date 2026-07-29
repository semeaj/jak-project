;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; Dropped code objects, MIPS code that cannot be raw-copied (their top-levels would
;; execute at link); they land with the jungle actor-code leg: jungle-part.o,
;; jungle-part2.o, jungle-obs.o, jungle-ocean.o. The effects pair landed with the
;; water leg (pure texture-anim-array data) and compiles from goal_src again.
("JUNGLEW.DGO"
 ("jungle-effects.o"
  "havjung-effects.o"
  "tpage-544.go"
  "jungle-flaming-arrow-ag.go"
  "jungle-branch-collision-a-ag.go"
  "junglew.go"
 ))
