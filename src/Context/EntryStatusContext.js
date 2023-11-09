import React, { createContext, useContext, useState } from "react";

const EntryStatusContext = createContext();

export const EntryStatusProvider = ({ children }) => {
  const [password, setPassword] = useState({
    Admin: 1080,
    otps: [3487, 6921, 1753, 8406, 5632, 2198, 4075, 9264, 5810, 3649],
    timestamp: 0,
  });

  const [isAdmin, setIsAdmin] = useState(false);
  const [isEntryCorrect, setIsEntryCorrect] = useState(false);

  const setCorrectEntryStatus = () => {
    setIsEntryCorrect(true);
  };

  const setIncorrectEntryStatus = () => {
    setIsEntryCorrect(false);
  };

  const setAdmin = () => {
    setIsAdmin(true);
  };

  const setTimestamp = () => {
    const tempState = { ...password };
    tempState.timestamp = Date.now();

    setPassword(tempState);
  };

  const setGuest = () => {
    setIsAdmin(false);
  };

  return (
    <EntryStatusContext.Provider
      value={{
        password,
        setPassword,
        isAdmin,
        setAdmin,
        setGuest,
        isEntryCorrect,
        setCorrectEntryStatus,
        setIncorrectEntryStatus,
        setTimestamp,
      }}
    >
      {children}
    </EntryStatusContext.Provider>
  );
};

export const useEntryStatus = () => {
  const context = useContext(EntryStatusContext);
  if (!context) {
    throw new Error(
      "useEntryStatus must be used within an EntryStatusProvider"
    );
  }
  return context;
};
