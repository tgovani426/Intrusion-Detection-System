import { useEffect } from "react";
import "./App.css";
import OTP_panel from "./components/OTP_panel";
import { useEntryStatus } from "./Context/EntryStatusContext";
import Home from "./components/Home";

function App() {
  const { isEntryCorrect } = useEntryStatus();

  useEffect(() => {
    // console.log(isEntryCorrect);
  }, [isEntryCorrect]);

  return (
    <div className="App">
      <div className="container">
        {isEntryCorrect ? <Home /> : <OTP_panel />}
      </div>
    </div>
  );
}

export default App;
