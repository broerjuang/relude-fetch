type fetchError = {
  url: string,
  innerError: Js.Promise.error,
};

type statusError = {
  url: string,
  actual: int,
  min: int,
  max: int,
};

type contentTypeError = {
  url: string,
  expected: ReludeFetch_Body.Type.t,
  innerError: Js.Promise.error,
};

type decodeError('innerError) = {
  url: string,
  innerError: 'innerError,
};

type customError('customError) = {
  url: string,
  innerError: 'customError,
};

type t('decodeError, 'customError) =
  | FetchError(fetchError)
  | StatusError(statusError)
  | ContentTypeError(contentTypeError)
  | DecodeError(decodeError('decodeError))
  | CustomError(customError('customError));

let fetchError = error => FetchError(error);

let statusError = error => StatusError(error);

let contentTypeError = error => ContentTypeError(error);

let decodeError = error => DecodeError(error);

let customError = error => CustomError(error);

let show:
  (
    'innerDecodeError => string,
    'innerCustomError => string,
    t('innerDecodeError, 'innerCustomError)
  ) =>
  string =
  (showDecodeError, showCustomError, error) =>
    switch (error) {
    | FetchError({url}) => "Fetch error: " ++ url

    | StatusError({url, actual, min, max}) =>
      "HTTP status error: "
      ++ url
      ++ ": "
      ++ Relude.Int.toString(actual)
      ++ ": Expected status between "
      ++ Relude.Int.toString(min)
      ++ " and "
      ++ Relude.Int.toString(max)

    | ContentTypeError({url, expected}) =>
      "Content type error: "
      ++ url
      ++ ": Expected content type: "
      ++ ReludeFetch_Body.Type.show(expected)

    | DecodeError({url, innerError}) =>
      "JSON decode error: "
      ++ url
      ++ ": Decode error: "
      ++ showDecodeError(innerError)
    | CustomError({url, innerError}) =>
      "Custom Error: "
      ++ url
      ++ "; Response error: "
      ++ showCustomError(innerError)
    };

