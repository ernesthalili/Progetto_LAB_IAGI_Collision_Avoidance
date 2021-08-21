
(cl:in-package :asdf)

(defsystem "collision_avoidance-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Istruzione" :depends-on ("_package_Istruzione"))
    (:file "_package_Istruzione" :depends-on ("_package"))
  ))