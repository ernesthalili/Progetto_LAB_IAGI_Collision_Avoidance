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
  :slots (_linear_velocity _angular_velocity ))

(defmethod collision_avoidance::Istruzione
  (:init
   (&key
    ((:linear_velocity __linear_velocity) 0.0)
    ((:angular_velocity __angular_velocity) 0.0)
    )
   (send-super :init)
   (setq _linear_velocity (float __linear_velocity))
   (setq _angular_velocity (float __angular_velocity))
   self)
  (:linear_velocity
   (&optional __linear_velocity)
   (if __linear_velocity (setq _linear_velocity __linear_velocity)) _linear_velocity)
  (:angular_velocity
   (&optional __angular_velocity)
   (if __angular_velocity (setq _angular_velocity __angular_velocity)) _angular_velocity)
  (:serialization-length
   ()
   (+
    ;; float32 _linear_velocity
    4
    ;; float32 _angular_velocity
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _linear_velocity
       (sys::poke _linear_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _angular_velocity
       (sys::poke _angular_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _linear_velocity
     (setq _linear_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _angular_velocity
     (setq _angular_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get collision_avoidance::Istruzione :md5sum-) "830d49d85ef543fb78a32609382932d6")
(setf (get collision_avoidance::Istruzione :datatype-) "collision_avoidance/Istruzione")
(setf (get collision_avoidance::Istruzione :definition-)
      "float32 linear_velocity
float32 angular_velocity

")



(provide :collision_avoidance/Istruzione "830d49d85ef543fb78a32609382932d6")


