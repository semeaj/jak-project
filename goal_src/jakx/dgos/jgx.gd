;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; All six jungle code objects now compile from goal_src, in the retail link order:
;; jungle-part, jungle-part2, jungle-obs, jungle-effects, jungle-ocean, havjung-effects.
;; jungle-obs.o (the jungle actor-code leg) must link ahead of the art groups so its
;; prop deftypes exist before the entity data interns those names; a 44-method data
;; stub widened by a later deftype asserts in kscheme. The bsp/vis object stays last.
("JGX.DGO"
 ("jungle-part.o"
  "jungle-part2.o"
  "jungle-obs.o"
  "jungle-effects.o"
  "jungle-ocean.o"
  "havjung-effects.o"
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
