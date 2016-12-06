import config from "./floatingpointconfig";


function log(value, base) {
  return Math.log(value) / Math.log(base);
}
function log2(value) {
  return log(value, 2);
}

function fRound(value) {
  let mantissaBits = config.mantissaBits;
  let exponentBits = config.exponentBits;

  let sign = Math.sign(value);
  let absValue = Math.abs(value);

  let exp = Math.floor(log2(absValue));
  let targetMantissa = absValue / Math.pow(2, exp);
  let mantissaValueLeft = targetMantissa - 1;

  //console.log(targetMantissa);

  let mantissa0 = 0;
  let exp0 = exp;

  for (let i = 1; i <= mantissaBits; i++) {
    let bitValue = Math.pow(2, -i);
    if (mantissaValueLeft - bitValue > 0) {
      mantissa0 += bitValue;
      mantissaValueLeft -= bitValue;
    }
  }

  let mantissa1 = mantissa0 + Math.pow(2, -mantissaBits);
  let exp1 = exp;
  
  if (mantissa1 > 1) {
    mantissa1 = 0;
    exp1++;
  }

  let value0 = sign * (1 + mantissa0) * Math.pow(2, exp);
  let value1 = sign * (1 + mantissa1) * Math.pow(2, exp);

  if (isNaN(value0) || isNaN(value1)) {
    debugger;
  }


  if (Math.abs(value0 - value) < Math.abs(value1 - value)) {
    return value0;
  } else {
    return value1;
  }
}



export default fRound;