const alertButton = document.getElementById('alertButton');
const alertDiv = document.getElementById('alertDiv');

alertButton.addEventListener('click', () => {
    alertDiv.classList.remove('hidden');
})