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
  :slots (_nome_utente _linear_velocity _angular_velocity ))

(defmethod collision_avoidance::Istruzione
  (:init
   (&key
    ((:nome_utente __nome_utente) "")
    ((:linear_velocity __linear_velocity) 0.0)
    ((:angular_velocity __angular_velocity) 0.0)
    )
   (send-super :init)
   (setq _nome_utente (string __nome_utente))
   (setq _linear_velocity (float __linear_velocity))
   (setq _angular_velocity (float __angular_velocity))
   self)
  (:nome_utente
   (&optional __nome_utente)
   (if __nome_utente (setq _nome_utente __nome_utente)) _nome_utente)
  (:linear_velocity
   (&optional __linear_velocity)
   (if __linear_velocity (setq _linear_velocity __linear_velocity)) _linear_velocity)
  (:angular_velocity
   (&optional __angular_velocity)
   (if __angular_velocity (setq _angular_velocity __angular_velocity)) _angular_velocity)
  (:serialization-length
   ()
   (+
    ;; string _nome_utente
    4 (length _nome_utente)
    ;; float32 _linear_velocity
    4
    ;; float32 _angular_velocity
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _nome_utente
       (write-long (length _nome_utente) s) (princ _nome_utente s)
     ;; float32 _linear_velocity
       (sys::poke _linear_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _angular_velocity
       (sys::poke _angular_velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _nome_utente
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _nome_utente (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float32 _linear_velocity
     (setq _linear_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _angular_velocity
     (setq _angular_velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get collision_avoidance::Istruzione :md5sum-) "0153c21ae9c9e8a4681e206c85c9df1f")
(setf (get collision_avoidance::Istruzione :datatype-) "collision_avoidance/Istruzione")
(setf (get collision_avoidance::Istruzione :definition-)
      "string nome_utente
float32 linear_velocity
float32 angular_velocity

")



(provide :collision_avoidance/Istruzione "0153c21ae9c9e8a4681e206c85c9df1f")


