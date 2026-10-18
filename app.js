'use strict';
const form = document.querySelector('#converter-form');
const input = document.querySelector('#account-id');
const result = document.querySelector('#result');
const copy = document.querySelector('#copy');
const message = document.querySelector('#convert-message');
input.addEventListener('input', () => { result.value = ''; copy.disabled = true; input.removeAttribute('aria-invalid'); message.textContent = 'An optional 0x prefix is accepted.'; });
form.addEventListener('submit', (event) => {
  event.preventDefault();
  const hex = input.value.trim().replace(/^0x/i, '');
  if (!/^[0-9a-f]{16}$/i.test(hex)) {
    result.value = ''; copy.disabled = true; input.setAttribute('aria-invalid', 'true');
    message.textContent = 'Enter exactly 16 hexadecimal characters using 0–9 and A–F.'; input.focus(); return;
  }
  input.removeAttribute('aria-invalid');
  result.value = btoa(hex.match(/../g).reverse().map(byte => String.fromCharCode(parseInt(byte, 16))).join(''));
  copy.disabled = false; message.textContent = 'Converted. Copy this value into the PSN Account-ID field in chiaki-ng.';
});
copy.addEventListener('click', async () => {
  try { await navigator.clipboard.writeText(result.value); message.textContent = 'Account-ID copied.'; }
  catch { result.focus(); result.select(); message.textContent = 'Result selected. Press Ctrl+C or use your device’s Copy action.'; }
});
