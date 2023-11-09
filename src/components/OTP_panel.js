import React, { useState, useRef } from "react";
import "./CSS/OTP_panel.css";

import { useEntryStatus } from "../Context/EntryStatusContext";

const OTPPanel = () => {
  const {
    setCorrectEntryStatus,
    setIncorrectEntryStatus,
    setAdmin,
    setGuest,
    password,
    
  } = useEntryStatus();

  const [otp, setOtp] = useState(["", "", "", ""]);
  // const [otp, setOtp] = useState([1, 2, 3, 4]);
  const inputRefs = [useRef(), useRef(), useRef(), useRef()];

  const handleChange = (index, value) => {
    const newOtp = [...otp];
    newOtp[index] = value;
    setOtp(newOtp);

    if (index < 3 && value !== "") {
      inputRefs[index + 1].current.focus();
    }
  };

  const checkPassword = () => {
    const pass = otp.join("");

    if (pass == password.Admin) {
      setCorrectEntryStatus();
      setAdmin();
      
      return
    }
    
    password.otps.map((otp) => {
      if(otp == pass){
        setCorrectEntryStatus()
        setGuest()
      
      }

    });

  };

  return (
    <div className="form">
      <div className="title">Verification Code</div>
      <p className="message">Please enter the Admin PIN or OTP to access the system</p>
      <div className="inputs">
        {otp.map((value, index) => (
          <input
            key={index}
            type="text"
            maxLength="1"
            value={value}
            onChange={(e) => handleChange(index, e.target.value)}
            ref={inputRefs[index]}
          />
        ))}
      </div>
      <div className="btn_container">
        <button type="button" onClick={checkPassword} className="action">
          Verify
        </button>
      </div>
    </div>
  );
};

export default OTPPanel;
