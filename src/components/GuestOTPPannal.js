import React, { useState } from "react";

import { useEntryStatus } from "../Context/EntryStatusContext";
import "./CSS/GuestOTPPannal.css";
const GuestOTPPannal = () => {
  const [guestOPT, setGuestOPT] = useState();
  const { password, addOTPtoLog } = useEntryStatus();

  const generateGuestOTP = () => {
    const random_num = Math.floor(Math.random() * 10);
    console.log(random_num);
    const newOTP = password.otps[random_num];
    setGuestOPT(newOTP);
  };

  return (
    <>
      <div className="GuestOTPPannal">
        <div style={{ marginLeft: "12px", marginBottom: "15px" }}>
          OTP for Guest user
        </div>

        {guestOPT ? (
          guestOPT
        ) : (
          <button type="button" className="otpBtn" onClick={generateGuestOTP}>
            <div class="svg-wrapper-1">
              <div class="svg-wrapper">
                <svg
                  xmlns="http://www.w3.org/2000/svg"
                  viewBox="0 0 24 24"
                  width="24"
                  height="24"
                >
                  <path fill="none" d="M0 0h24v24H0z"></path>
                  <path
                    fill="currentColor"
                    d="M1.946 9.315c-.522-.174-.527-.455.01-.634l19.087-6.362c.529-.176.832.12.684.638l-5.454 19.086c-.15.529-.455.547-.679.045L12 14l6-8-8 6-8.054-2.685z"
                  ></path>
                </svg>
              </div>
            </div>
            <span>Generate OTP</span>
          </button>

          // <button type="button" onClick={generateGuestOTP}>
          //   Generate OTP
          // </button>
        )}
      </div>
    </>
  );
};

export default GuestOTPPannal;
