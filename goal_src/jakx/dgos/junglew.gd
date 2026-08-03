;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; All six jungle code objects now compile from goal_src, in the retail link order:
;; jungle-part, jungle-part2, jungle-obs, jungle-effects, jungle-ocean, havjung-effects.
;; jungle-obs.o (the jungle actor-code leg) must link ahead of the art and bsp data so
;; its prop deftypes exist before the entity data interns those names; a 44-method data
;; stub widened by a later deftype asserts in kscheme. The bsp object stays last.
("JUNGLEW.DGO"
 ("jungle-part.o"
  "jungle-part2.o"
  "jungle-obs.o"
  "jungle-effects.o"
  "jungle-ocean.o"
  "havjung-effects.o"
  "tpage-544.go"
  "jungle-flaming-arrow-ag.go"
  "jungle-branch-collision-a-ag.go"
  "junglew.go"
 ))
