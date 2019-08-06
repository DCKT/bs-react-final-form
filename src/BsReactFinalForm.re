module type FormConfig = {type state;};

module FinalForm = {
  module Meta = {
    type t;

    [@bs.get] external touched: t => bool = "touched";
    [@bs.get] external active: t => bool = "active";
    [@bs.get] external dirty: t => bool = "dirty";
    [@bs.get] external invalid: t => bool = "invalid";
    [@bs.get] external pristine: t => bool = "pristine";
    [@bs.get] external submitFailed: t => bool = "submitFailed";
    [@bs.get] external submitSucceeded: t => bool = "submitSucceeded";
    [@bs.get] external submitting: t => bool = "submitting";
    [@bs.get] external valid: t => bool = "valid";
    [@bs.get] external validating: t => bool = "validating";
    [@bs.get] external visited: t => bool = "visited";
    [@bs.get] external modified: t => bool = "modified";
    [@bs.get] external initial: t => 'a = "initial";
    [@bs.get] external error: t => 'a = "error";
  };

  module Input = {
    type t;

    [@bs.send] external onChange: (t, ReactEvent.Form.t) => unit = "onChange";
    [@bs.get] external onBlur: t => (. ReactEvent.Form.t) => unit = "onBlur";
    [@bs.get] external onFocus: t => (. ReactEvent.Form.t) => unit = "onFocus";
    [@bs.get] external value: t => 'a = "value";
    [@bs.get] external name: t => string = "name";
  };

  module FieldsRenderProps = {
    [@bs.deriving abstract]
    type t = {
      input: Input.t,
      meta: Meta.t,
    };
  };

  module FieldsProps = {
    [@bs.deriving abstract]
    type t('a) = {
      [@bs.optional]
      placeholder: string,
      [@bs.optional]
      allowNull: bool,
      [@bs.optional]
      initialValue: 'a,
    };
  };

  module FormRenderProps = {
    type t;

    [@bs.send]
    external handleSubmit: (t, ReactEvent.Form.t) => 'a = "handleSubmit";
  };

  module Make = (Config: FormConfig) => {
    module Form = {
      [@bs.module "react-final-form"] [@react.component]
      external make:
        (
          ~initialValues: option(Config.state)=?,
          ~onSubmit: Config.state => Js.Promise.t('a),
          ~render: FormRenderProps.t => React.element
        ) =>
        React.element =
        "Form";
    };

    module Field = {
      type component('a) =
        | String: component(string)
        | Element: component(React.element);

      [@bs.module "react-final-form"] [@react.component]
      external make:
        (
          ~name: string,
          ~placeholder: string=?,
          ~render: (~input: Input.t, ~meta: Meta.t) => React.element=?,
          ~formatOnBlur: bool=?,
          ~component: 'a=?
        ) =>
        React.element =
        "Field";

      let makeProps =
          (
            ~name,
            ~placeholder=?,
            ~formatOnBlur=?,
            ~component: option((component('a), 'a))=?,
          ) =>
        makeProps(
          ~name,
          ~placeholder?,
          ~formatOnBlur?,
          ~component=?Belt.Option.map(component, (_, v) => v),
        );
    };

    [@bs.module "react-final-form"]
    external useField:
      (~name: string, ~options: FieldsProps.t('a)=?, unit) =>
      FieldsRenderProps.t =
      "useField";
  };
};