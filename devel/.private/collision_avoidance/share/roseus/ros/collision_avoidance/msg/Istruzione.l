;; Auto-generated. Do not edit!


(when (boundp 'collision_avoidance::Istruzione)
  (if (not (find-package "COLLISION_AVOIDANCE"))
    (make-package "COLLISION_AVOIDANCE"))
  (shadow 'Istruzione (find-package "COLLISION_AVOIDANCE")))
(unless (find-package "COLLISION_AVOIDANCE::ISTRUZIONE")
  (make-package "COLLISION_AVOIDANCE::ISTRUZIONE"))

(in-package "ROS")
;;//! \htmlinclude Istruzione.msg.html


(defclass collision_avoidance::Istruzione
  :super ros::object
  :slots (_partito _linear_velocity _angular_velocity ))

(defmethod collision_avoidance::Istruzione
  (:init
   (&key
    ((:partito __partito) nil)
    ((:linear_velocity __linear_velocity) 0.0)
    ((:angular_velocity __angular_velocity) 0.0)
    )
   (send-super :init)
   (setq _partito __partito)
   (setq _linear_velocity (float __linear_velocity))
   (setq _angular_velocity (float __angular_velocity))
   self)
  (:partito
   (&optional __partito)
   (if __partito (setq _partito __partito)) _partito)
  (:linear_velocity
   (&optional __linear_velocity)
   (if __linear_velocity (setq _linear_velocity __linear_velocity)) _linear_velocity)
  (:angular_velocity
   (&optional __angular_velocity)
   (if __angular_velocity (setq _angular_velocity __angular_velocity)) _angular_velocity)
  (:serialization-length
   ()
   (+
    ;; bool _partito
    1
    ;; float32 _linear_velocity
    4
    ;; float32 _angular_velocity
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _partito
       (if _partito (write-byte -1 s) (write-byte 0 s))
     ;; float32 _linear_velocity
       (sys::poke _linear_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _angular_velocity
       (sys::poke _angular_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _partito
     (setq _partito (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; float32 _linear_velocity
     (setq _linear_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _angular_velocity
     (setq _angular_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get collision_avoidance::Istruzione :md5sum-) "f21bc4c4d0519cf3ffa674002fbcf006")
(setf (get collision_avoidance::Istruzione :datatype-) "collision_avoidance/Istruzione")
(setf (get collision_avoidance::Istruzione :definition-)
      "bool partito
float32 linear_velocity
float32 angular_velocity

")



(provide :collision_avoidance/Istruzione "f21bc4c4d0519cf3ffa674002fbcf006")


