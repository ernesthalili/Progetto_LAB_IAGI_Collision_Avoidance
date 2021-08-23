; Auto-generated. Do not edit!


(cl:in-package collision_avoidance-msg)


;//! \htmlinclude Istruzione.msg.html

(cl:defclass <Istruzione> (roslisp-msg-protocol:ros-message)
  ((nome_utente
    :reader nome_utente
    :initarg :nome_utente
    :type cl:string
    :initform "")
   (linear_velocity
    :reader linear_velocity
    :initarg :linear_velocity
    :type cl:float
    :initform 0.0)
   (angular_velocity
    :reader angular_velocity
    :initarg :angular_velocity
    :type cl:float
    :initform 0.0))
)

(cl:defclass Istruzione (<Istruzione>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Istruzione>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Istruzione)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name collision_avoidance-msg:<Istruzione> is deprecated: use collision_avoidance-msg:Istruzione instead.")))

(cl:ensure-generic-function 'nome_utente-val :lambda-list '(m))
(cl:defmethod nome_utente-val ((m <Istruzione>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader collision_avoidance-msg:nome_utente-val is deprecated.  Use collision_avoidance-msg:nome_utente instead.")
  (nome_utente m))

(cl:ensure-generic-function 'linear_velocity-val :lambda-list '(m))
(cl:defmethod linear_velocity-val ((m <Istruzione>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader collision_avoidance-msg:linear_velocity-val is deprecated.  Use collision_avoidance-msg:linear_velocity instead.")
  (linear_velocity m))

(cl:ensure-generic-function 'angular_velocity-val :lambda-list '(m))
(cl:defmethod angular_velocity-val ((m <Istruzione>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader collision_avoidance-msg:angular_velocity-val is deprecated.  Use collision_avoidance-msg:angular_velocity instead.")
  (angular_velocity m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Istruzione>) ostream)
  "Serializes a message object of type '<Istruzione>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'nome_utente))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'nome_utente))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'linear_velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'angular_velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Istruzione>) istream)
  "Deserializes a message object of type '<Istruzione>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'nome_utente) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'nome_utente) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'linear_velocity) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angular_velocity) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Istruzione>)))
  "Returns string type for a message object of type '<Istruzione>"
  "collision_avoidance/Istruzione")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Istruzione)))
  "Returns string type for a message object of type 'Istruzione"
  "collision_avoidance/Istruzione")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Istruzione>)))
  "Returns md5sum for a message object of type '<Istruzione>"
  "0153c21ae9c9e8a4681e206c85c9df1f")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Istruzione)))
  "Returns md5sum for a message object of type 'Istruzione"
  "0153c21ae9c9e8a4681e206c85c9df1f")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Istruzione>)))
  "Returns full string definition for message of type '<Istruzione>"
  (cl:format cl:nil "string nome_utente~%float32 linear_velocity~%float32 angular_velocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Istruzione)))
  "Returns full string definition for message of type 'Istruzione"
  (cl:format cl:nil "string nome_utente~%float32 linear_velocity~%float32 angular_velocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Istruzione>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'nome_utente))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Istruzione>))
  "Converts a ROS message object to a list"
  (cl:list 'Istruzione
    (cl:cons ':nome_utente (nome_utente msg))
    (cl:cons ':linear_velocity (linear_velocity msg))
    (cl:cons ':angular_velocity (angular_velocity msg))
))
