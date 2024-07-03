 <!DOCTYPE html>
<html>
<head>
  <script type='module'>
    
    // Import Firebase methods
    import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.8.0/firebase-app.js';
    import { getDatabase, ref, child, onValue, set } from 'https://www.gstatic.com/firebasejs/10.8.0/firebase-database.js';

    // Initialize Firebase
    const config = { databaseURL: 'https://long-bc130-default-rtdb.asia-southeast1.firebasedatabase.app/' };
    initializeApp(config);

    // Reference to the database
    const dbRef = ref(getDatabase());

    // Event handler when the page is loaded
    window.page_Load = function page_Load() {
      onValue(child(dbRef, 'RelayStatus'), (snapshot) => {
        const relayStatus = snapshot.val();
        updateRelayStatusDisplay(relayStatus);
      });
    }

    // Update relay status display and LED colors
    window.updateRelayStatusDisplay = function updateRelayStatusDisplay(status) {
      const lblValue = document.getElementById('lblValue');
      const redLed = document.getElementById('redLed');
      const blueLed = document.getElementById('greenLed');

      lblValue.innerText = status;
      if (status == 1) {
        redLed.style.backgroundColor = 'red';
        blueLed.style.backgroundColor = 'white';
      } else {
        redLed.style.backgroundColor = 'white';
        blueLed.style.backgroundColor = 'green';
      }
    }

    // Event handler when the "ON/OFF" button is clicked
    window.btnOnOff_Click = function btnOnOff_Click() {
      const currentValue = parseInt(document.getElementById('lblValue').innerText);
      const newValue = currentValue === 1 ? 0 : 1; // Toggle between 0 and 1
      set(child(dbRef, 'RelayStatus'), newValue)
        .then(() => console.log('Value set successfully:', newValue))
        .catch(error => console.error('Error setting value:', error));
    }
  </script>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js"></script>
  
</head>

<div class="p-5 bg-primary text-white text-center">
  <h1>IOT PROJECT</h1>
  <p>Chào Mừng Các Bạn Đã Đến Với Bài Thuyết Trình Của Chúng Tôi!</p> 
</div>

<nav class="navbar navbar-expand-sm bg-dark navbar-dark">
  <div class="container-fluid">
    <ul class="navbar-nav">
      <li class="nav-item">
        <a class="nav-link active" href="#">Trang Chủ</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="#">Sản Phẩm</a>
      </li>
      <li class="nav-item">
        <a class="nav-link" href="#">Demo</a>
      </li>
      <li class="nav-item">
        <a class="nav-link disabled" href="#">Thành Viên</a>
      </li>
    </ul>
  </div>
</nav>


<body onload='page_Load()'>
  <table cellspacing='10'>
    <tr>
      <td colspan='3'>Relay Control</td>
    </tr>
    <tr>
      <td>Value:</td>
      <td><span id='lblValue'></span></td>
      <td><button onclick='btnOnOff_Click()'>ON / OFF</button></td>
    </tr>
    <tr>
      <td>Red LED:</td>
      <td><div id='redLed' style='width:20px;height:20px;border:1px solid #000;'></div></td>
    </tr>
    <tr>
      <td>Blue LED:</td>
      <td><div id='greenLed' style='width:20px;height:20px;border:1px solid #000;'></div></td>
    </tr>
  </table>
  </body>
</html>
