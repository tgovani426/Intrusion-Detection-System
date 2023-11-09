import React, { useEffect, useState } from "react";

import "./CSS/Home.css";
import GuestOTPPannal from "./GuestOTPPannal";
import { useEntryStatus } from "../Context/EntryStatusContext";
function Home() {
  const [socket, setSocket] = useState(null);
  const { isAdmin, setTimestamp, password } = useEntryStatus();
  const [isIntrusion, setIsIntrusion] = useState(false);

  useEffect(() => {
    const newSocket = new WebSocket("ws://192.168.114.238:81");
    setSocket(newSocket);

    newSocket.onopen = () => {
      console.log("Esp8266 Connected....");
    };

    newSocket.onmessage = (event) => {
      const msg = event.data;
      // console.log("Received message: " + typeof msg.trim());
      // Incoming message can be processed here

      if (parseInt(msg) == 2) {
        setIsIntrusion(true);
      }
    };

    return () => {
      newSocket.close();
    };
  }, []);

  const handleRequest = (message) => {
    if (socket) {
      message == "1" || message == "0" ? setTimestamp() : console.log();
      socket.send(message);
    }
  };
  useEffect(() => {
    // Compare the incoming request time.

    if (isIntrusion == true) {
      console.log(Date.now());
      console.log(password.timestamp);
      if (Date.now() - password.timestamp > 0.5 * 60 * 1000) {
        handleRequest("3");
        window.alert("Intrusion Detected !!!")
        setIsIntrusion(false);
      }
      else{
        handleRequest('4')
        setIsIntrusion(false)
      }
    }
  }, [isIntrusion]);

  return (
    <>
      <div className="container">
        <div style={{display:"flex"}}>
          <button
            type="button"
            onClick={() => handleRequest(1)}
            className="open-button"
          >
            Unlock
          </button>
          <button
            type="button"
            onClick={() => handleRequest(0)}
            className="close-button"
          >
            Lock
          </button>
        </div>

        {isAdmin && (
          <div className="GuestOTPPannal">
            <GuestOTPPannal />
          </div>
        )}
      </div>
    </>
  );
}

export default Home;
