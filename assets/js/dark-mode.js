document.addEventListener("DOMContentLoaded", function () {
  const toggleButton = document.getElementById("dark-mode-toggle");
  const body = document.body;

  // Check user preference in localStorage
  if (localStorage.getItem("dark-mode") === "enabled") {
    body.classList.add("dark-mode");
  }

  toggleButton.addEventListener("click", function () {
    body.classList.toggle("dark-mode");

    // Save preference in localStorage
    if (body.classList.contains("dark-mode")) {
      localStorage.setItem("dark-mode", "enabled");
    } else {
      localStorage.setItem("dark-mode", "disabled");
    }
  });
});
