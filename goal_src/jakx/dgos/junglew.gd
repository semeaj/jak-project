;; og:preserve-this bring-up: data entries converted to .go raw copies (brdroom recipe).
;; All six jungle code objects now compile from goal_src, in the retail link order:
;; jungle-part, jungle-part2, jungle-obs, jungle-effects, jungle-ocean, havjung-effects.
;; jungle-obs.o (the jungle actor-code leg) must link ahead of the art and bsp data so
;; its prop deftypes exist before the entity data interns those names; a data stub (12
;; methods for these v5 links) widened by a later deftype asserts in kscheme. The bsp
;; object stays last. Cross-DGO reliance: seven OTHER built jungle section/vista DGOs
;; (JGA/JGB/JGC/JGD/JGE/JGG/JGY) carry these same prop type names as bare bsp data
;; links with NO code object, so within-DGO order cannot protect them; a section-first
;; load interns the stub globally and the next jungle-obs.o link asserts. Want-set
;; discipline (the junglew/jgx code DGOs load first at every jungle continue point) is
;; what holds that at bay; see the forge #51 note in game/kernel/jakx/kscheme.cpp.
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
