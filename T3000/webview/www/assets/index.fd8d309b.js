var jl=Object.defineProperty,Bl=Object.defineProperties;var Nl=Object.getOwnPropertyDescriptors;var dn=Object.getOwnPropertySymbols;var Ll=Object.prototype.hasOwnProperty,Hl=Object.prototype.propertyIsEnumerable;var pn=(t,e,i)=>e in t?jl(t,e,{enumerable:!0,configurable:!0,writable:!0,value:i}):t[e]=i,j0=(t,e)=>{for(var i in e||(e={}))Ll.call(e,i)&&pn(t,i,e[i]);if(dn)for(var i of dn(e))Hl.call(e,i)&&pn(t,i,e[i]);return t},Ft=(t,e)=>Bl(t,Nl(e));function Es(t,e){const i=Object.create(null),s=t.split(",");for(let n=0;n<s.length;n++)i[s[n]]=!0;return e?n=>!!i[n.toLowerCase()]:n=>!!i[n]}const Ul="itemscope,allowfullscreen,formnovalidate,ismap,nomodule,novalidate,readonly",Vl=Es(Ul);function Z2(t){return!!t||t===""}function Ss(t){if(q(t)){const e={};for(let i=0;i<t.length;i++){const s=t[i],n=we(s)?Kl(s):Ss(s);if(n)for(const r in n)e[r]=n[r]}return e}else{if(we(t))return t;if(be(t))return t}}const Gl=/;(?![^(]*\))/g,ql=/:(.+)/;function Kl(t){const e={};return t.split(Gl).forEach(i=>{if(i){const s=i.split(ql);s.length>1&&(e[s[0].trim()]=s[1].trim())}}),e}function Ps(t){let e="";if(we(t))e=t;else if(q(t))for(let i=0;i<t.length;i++){const s=Ps(t[i]);s&&(e+=s+" ")}else if(be(t))for(const i in t)t[i]&&(e+=i+" ");return e.trim()}const Xc=t=>we(t)?t:t==null?"":q(t)||be(t)&&(t.toString===tr||!Y(t.toString))?JSON.stringify(t,J2,2):String(t),J2=(t,e)=>e&&e.__v_isRef?J2(t,e.value):mt(e)?{[`Map(${e.size})`]:[...e.entries()].reduce((i,[s,n])=>(i[`${s} =>`]=n,i),{})}:Q2(e)?{[`Set(${e.size})`]:[...e.values()]}:be(e)&&!q(e)&&!ir(e)?String(e):e,de={},ut=[],Ze=()=>{},Xl=()=>!1,Wl=/^on[^a-z]/,si=t=>Wl.test(t),Ds=t=>t.startsWith("onUpdate:"),Pe=Object.assign,As=(t,e)=>{const i=t.indexOf(e);i>-1&&t.splice(i,1)},Yl=Object.prototype.hasOwnProperty,se=(t,e)=>Yl.call(t,e),q=Array.isArray,mt=t=>ni(t)==="[object Map]",Q2=t=>ni(t)==="[object Set]",Y=t=>typeof t=="function",we=t=>typeof t=="string",Os=t=>typeof t=="symbol",be=t=>t!==null&&typeof t=="object",er=t=>be(t)&&Y(t.then)&&Y(t.catch),tr=Object.prototype.toString,ni=t=>tr.call(t),Zl=t=>ni(t).slice(8,-1),ir=t=>ni(t)==="[object Object]",Rs=t=>we(t)&&t!=="NaN"&&t[0]!=="-"&&""+parseInt(t,10)===t,I1=Es(",key,ref,ref_for,ref_key,onVnodeBeforeMount,onVnodeMounted,onVnodeBeforeUpdate,onVnodeUpdated,onVnodeBeforeUnmount,onVnodeUnmounted"),ri=t=>{const e=Object.create(null);return i=>e[i]||(e[i]=t(i))},Jl=/-(\w)/g,a0=ri(t=>t.replace(Jl,(e,i)=>i?i.toUpperCase():"")),Ql=/\B([A-Z])/g,Pt=ri(t=>t.replace(Ql,"-$1").toLowerCase()),li=ri(t=>t.charAt(0).toUpperCase()+t.slice(1)),Pi=ri(t=>t?`on${li(t)}`:""),e1=(t,e)=>!Object.is(t,e),Vt=(t,e)=>{for(let i=0;i<t.length;i++)t[i](e)},H1=(t,e,i)=>{Object.defineProperty(t,e,{configurable:!0,enumerable:!1,value:i})},sr=t=>{const e=parseFloat(t);return isNaN(e)?t:e};let un;const e4=()=>un||(un=typeof globalThis!="undefined"?globalThis:typeof self!="undefined"?self:typeof window!="undefined"?window:typeof global!="undefined"?global:{});let n0;class nr{constructor(e=!1){this.active=!0,this.effects=[],this.cleanups=[],!e&&n0&&(this.parent=n0,this.index=(n0.scopes||(n0.scopes=[])).push(this)-1)}run(e){if(this.active){const i=n0;try{return n0=this,e()}finally{n0=i}}}on(){n0=this}off(){n0=this.parent}stop(e){if(this.active){let i,s;for(i=0,s=this.effects.length;i<s;i++)this.effects[i].stop();for(i=0,s=this.cleanups.length;i<s;i++)this.cleanups[i]();if(this.scopes)for(i=0,s=this.scopes.length;i<s;i++)this.scopes[i].stop(!0);if(this.parent&&!e){const n=this.parent.scopes.pop();n&&n!==this&&(this.parent.scopes[this.index]=n,n.index=this.index)}this.active=!1}}}function t4(t){return new nr(t)}function i4(t,e=n0){e&&e.active&&e.effects.push(t)}const Ts=t=>{const e=new Set(t);return e.w=0,e.n=0,e},rr=t=>(t.w&D0)>0,lr=t=>(t.n&D0)>0,s4=({deps:t})=>{if(t.length)for(let e=0;e<t.length;e++)t[e].w|=D0},n4=t=>{const{deps:e}=t;if(e.length){let i=0;for(let s=0;s<e.length;s++){const n=e[s];rr(n)&&!lr(n)?n.delete(t):e[i++]=n,n.w&=~D0,n.n&=~D0}e.length=i}},Ji=new WeakMap;let Lt=0,D0=1;const Qi=30;let Xe;const X0=Symbol(""),es=Symbol("");class Ms{constructor(e,i=null,s){this.fn=e,this.scheduler=i,this.active=!0,this.deps=[],this.parent=void 0,i4(this,s)}run(){if(!this.active)return this.fn();let e=Xe,i=E0;for(;e;){if(e===this)return;e=e.parent}try{return this.parent=Xe,Xe=this,E0=!0,D0=1<<++Lt,Lt<=Qi?s4(this):mn(this),this.fn()}finally{Lt<=Qi&&n4(this),D0=1<<--Lt,Xe=this.parent,E0=i,this.parent=void 0,this.deferStop&&this.stop()}}stop(){Xe===this?this.deferStop=!0:this.active&&(mn(this),this.onStop&&this.onStop(),this.active=!1)}}function mn(t){const{deps:e}=t;if(e.length){for(let i=0;i<e.length;i++)e[i].delete(t);e.length=0}}let E0=!0;const or=[];function Dt(){or.push(E0),E0=!1}function At(){const t=or.pop();E0=t===void 0?!0:t}function Ne(t,e,i){if(E0&&Xe){let s=Ji.get(t);s||Ji.set(t,s=new Map);let n=s.get(i);n||s.set(i,n=Ts()),ar(n)}}function ar(t,e){let i=!1;Lt<=Qi?lr(t)||(t.n|=D0,i=!rr(t)):i=!t.has(Xe),i&&(t.add(Xe),Xe.deps.push(t))}function p0(t,e,i,s,n,r){const l=Ji.get(t);if(!l)return;let o=[];if(e==="clear")o=[...l.values()];else if(i==="length"&&q(t))l.forEach((a,c)=>{(c==="length"||c>=s)&&o.push(a)});else switch(i!==void 0&&o.push(l.get(i)),e){case"add":q(t)?Rs(i)&&o.push(l.get("length")):(o.push(l.get(X0)),mt(t)&&o.push(l.get(es)));break;case"delete":q(t)||(o.push(l.get(X0)),mt(t)&&o.push(l.get(es)));break;case"set":mt(t)&&o.push(l.get(X0));break}if(o.length===1)o[0]&&ts(o[0]);else{const a=[];for(const c of o)c&&a.push(...c);ts(Ts(a))}}function ts(t,e){const i=q(t)?t:[...t];for(const s of i)s.computed&&gn(s);for(const s of i)s.computed||gn(s)}function gn(t,e){(t!==Xe||t.allowRecurse)&&(t.scheduler?t.scheduler():t.run())}const r4=Es("__proto__,__v_isRef,__isVue"),cr=new Set(Object.getOwnPropertyNames(Symbol).filter(t=>t!=="arguments"&&t!=="caller").map(t=>Symbol[t]).filter(Os)),l4=Fs(),o4=Fs(!1,!0),a4=Fs(!0),yn=c4();function c4(){const t={};return["includes","indexOf","lastIndexOf"].forEach(e=>{t[e]=function(...i){const s=re(this);for(let r=0,l=this.length;r<l;r++)Ne(s,"get",r+"");const n=s[e](...i);return n===-1||n===!1?s[e](...i.map(re)):n}}),["push","pop","shift","unshift","splice"].forEach(e=>{t[e]=function(...i){Dt();const s=re(this)[e].apply(this,i);return At(),s}}),t}function Fs(t=!1,e=!1){return function(s,n,r){if(n==="__v_isReactive")return!t;if(n==="__v_isReadonly")return t;if(n==="__v_isShallow")return e;if(n==="__v_raw"&&r===(t?e?C4:ur:e?pr:dr).get(s))return s;const l=q(s);if(!t&&l&&se(yn,n))return Reflect.get(yn,n,r);const o=Reflect.get(s,n,r);return(Os(n)?cr.has(n):r4(n))||(t||Ne(s,"get",n),e)?o:Se(o)?l&&Rs(n)?o:o.value:be(o)?t?mr(o):Q0(o):o}}const h4=hr(),f4=hr(!0);function hr(t=!1){return function(i,s,n,r){let l=i[s];if(t1(l)&&Se(l)&&!Se(n))return!1;if(!t&&!t1(n)&&(is(n)||(n=re(n),l=re(l)),!q(i)&&Se(l)&&!Se(n)))return l.value=n,!0;const o=q(i)&&Rs(s)?Number(s)<i.length:se(i,s),a=Reflect.set(i,s,n,r);return i===re(r)&&(o?e1(n,l)&&p0(i,"set",s,n):p0(i,"add",s,n)),a}}function d4(t,e){const i=se(t,e);t[e];const s=Reflect.deleteProperty(t,e);return s&&i&&p0(t,"delete",e,void 0),s}function p4(t,e){const i=Reflect.has(t,e);return(!Os(e)||!cr.has(e))&&Ne(t,"has",e),i}function u4(t){return Ne(t,"iterate",q(t)?"length":X0),Reflect.ownKeys(t)}const fr={get:l4,set:h4,deleteProperty:d4,has:p4,ownKeys:u4},m4={get:a4,set(t,e){return!0},deleteProperty(t,e){return!0}},g4=Pe({},fr,{get:o4,set:f4}),Is=t=>t,oi=t=>Reflect.getPrototypeOf(t);function _1(t,e,i=!1,s=!1){t=t.__v_raw;const n=re(t),r=re(e);i||(e!==r&&Ne(n,"get",e),Ne(n,"get",r));const{has:l}=oi(n),o=s?Is:i?Ns:i1;if(l.call(n,e))return o(t.get(e));if(l.call(n,r))return o(t.get(r));t!==n&&t.get(e)}function C1(t,e=!1){const i=this.__v_raw,s=re(i),n=re(t);return e||(t!==n&&Ne(s,"has",t),Ne(s,"has",n)),t===n?i.has(t):i.has(t)||i.has(n)}function $1(t,e=!1){return t=t.__v_raw,!e&&Ne(re(t),"iterate",X0),Reflect.get(t,"size",t)}function xn(t){t=re(t);const e=re(this);return oi(e).has.call(e,t)||(e.add(t),p0(e,"add",t,t)),this}function vn(t,e){e=re(e);const i=re(this),{has:s,get:n}=oi(i);let r=s.call(i,t);r||(t=re(t),r=s.call(i,t));const l=n.call(i,t);return i.set(t,e),r?e1(e,l)&&p0(i,"set",t,e):p0(i,"add",t,e),this}function wn(t){const e=re(this),{has:i,get:s}=oi(e);let n=i.call(e,t);n||(t=re(t),n=i.call(e,t)),s&&s.call(e,t);const r=e.delete(t);return n&&p0(e,"delete",t,void 0),r}function bn(){const t=re(this),e=t.size!==0,i=t.clear();return e&&p0(t,"clear",void 0,void 0),i}function E1(t,e){return function(s,n){const r=this,l=r.__v_raw,o=re(l),a=e?Is:t?Ns:i1;return!t&&Ne(o,"iterate",X0),l.forEach((c,h)=>s.call(n,a(c),a(h),r))}}function S1(t,e,i){return function(...s){const n=this.__v_raw,r=re(n),l=mt(r),o=t==="entries"||t===Symbol.iterator&&l,a=t==="keys"&&l,c=n[t](...s),h=i?Is:e?Ns:i1;return!e&&Ne(r,"iterate",a?es:X0),{next(){const{value:p,done:d}=c.next();return d?{value:p,done:d}:{value:o?[h(p[0]),h(p[1])]:h(p),done:d}},[Symbol.iterator](){return this}}}}function y0(t){return function(...e){return t==="delete"?!1:this}}function y4(){const t={get(r){return _1(this,r)},get size(){return $1(this)},has:C1,add:xn,set:vn,delete:wn,clear:bn,forEach:E1(!1,!1)},e={get(r){return _1(this,r,!1,!0)},get size(){return $1(this)},has:C1,add:xn,set:vn,delete:wn,clear:bn,forEach:E1(!1,!0)},i={get(r){return _1(this,r,!0)},get size(){return $1(this,!0)},has(r){return C1.call(this,r,!0)},add:y0("add"),set:y0("set"),delete:y0("delete"),clear:y0("clear"),forEach:E1(!0,!1)},s={get(r){return _1(this,r,!0,!0)},get size(){return $1(this,!0)},has(r){return C1.call(this,r,!0)},add:y0("add"),set:y0("set"),delete:y0("delete"),clear:y0("clear"),forEach:E1(!0,!0)};return["keys","values","entries",Symbol.iterator].forEach(r=>{t[r]=S1(r,!1,!1),i[r]=S1(r,!0,!1),e[r]=S1(r,!1,!0),s[r]=S1(r,!0,!0)}),[t,i,e,s]}const[x4,v4,w4,b4]=y4();function js(t,e){const i=e?t?b4:w4:t?v4:x4;return(s,n,r)=>n==="__v_isReactive"?!t:n==="__v_isReadonly"?t:n==="__v_raw"?s:Reflect.get(se(i,n)&&n in s?i:s,n,r)}const k4={get:js(!1,!1)},z4={get:js(!1,!0)},_4={get:js(!0,!1)},dr=new WeakMap,pr=new WeakMap,ur=new WeakMap,C4=new WeakMap;function $4(t){switch(t){case"Object":case"Array":return 1;case"Map":case"Set":case"WeakMap":case"WeakSet":return 2;default:return 0}}function E4(t){return t.__v_skip||!Object.isExtensible(t)?0:$4(Zl(t))}function Q0(t){return t1(t)?t:Bs(t,!1,fr,k4,dr)}function S4(t){return Bs(t,!1,g4,z4,pr)}function mr(t){return Bs(t,!0,m4,_4,ur)}function Bs(t,e,i,s,n){if(!be(t)||t.__v_raw&&!(e&&t.__v_isReactive))return t;const r=n.get(t);if(r)return r;const l=E4(t);if(l===0)return t;const o=new Proxy(t,l===2?s:i);return n.set(t,o),o}function gt(t){return t1(t)?gt(t.__v_raw):!!(t&&t.__v_isReactive)}function t1(t){return!!(t&&t.__v_isReadonly)}function is(t){return!!(t&&t.__v_isShallow)}function gr(t){return gt(t)||t1(t)}function re(t){const e=t&&t.__v_raw;return e?re(e):t}function ai(t){return H1(t,"__v_skip",!0),t}const i1=t=>be(t)?Q0(t):t,Ns=t=>be(t)?mr(t):t;function yr(t){E0&&Xe&&(t=re(t),ar(t.dep||(t.dep=Ts())))}function xr(t,e){t=re(t),t.dep&&ts(t.dep)}function Se(t){return!!(t&&t.__v_isRef===!0)}function Ls(t){return vr(t,!1)}function P4(t){return vr(t,!0)}function vr(t,e){return Se(t)?t:new D4(t,e)}class D4{constructor(e,i){this.__v_isShallow=i,this.dep=void 0,this.__v_isRef=!0,this._rawValue=i?e:re(e),this._value=i?e:i1(e)}get value(){return yr(this),this._value}set value(e){e=this.__v_isShallow?e:re(e),e1(e,this._rawValue)&&(this._rawValue=e,this._value=this.__v_isShallow?e:i1(e),xr(this))}}function Gt(t){return Se(t)?t.value:t}const A4={get:(t,e,i)=>Gt(Reflect.get(t,e,i)),set:(t,e,i,s)=>{const n=t[e];return Se(n)&&!Se(i)?(n.value=i,!0):Reflect.set(t,e,i,s)}};function wr(t){return gt(t)?t:new Proxy(t,A4)}class O4{constructor(e,i,s,n){this._setter=i,this.dep=void 0,this.__v_isRef=!0,this._dirty=!0,this.effect=new Ms(e,()=>{this._dirty||(this._dirty=!0,xr(this))}),this.effect.computed=this,this.effect.active=this._cacheable=!n,this.__v_isReadonly=s}get value(){const e=re(this);return yr(e),(e._dirty||!e._cacheable)&&(e._dirty=!1,e._value=e.effect.run()),e._value}set value(e){this._setter(e)}}function R4(t,e,i=!1){let s,n;const r=Y(t);return r?(s=t,n=Ze):(s=t.get,n=t.set),new O4(s,n,r||!n,i)}function S0(t,e,i,s){let n;try{n=s?t(...s):t()}catch(r){ci(r,e,i)}return n}function Ge(t,e,i,s){if(Y(t)){const r=S0(t,e,i,s);return r&&er(r)&&r.catch(l=>{ci(l,e,i)}),r}const n=[];for(let r=0;r<t.length;r++)n.push(Ge(t[r],e,i,s));return n}function ci(t,e,i,s=!0){const n=e?e.vnode:null;if(e){let r=e.parent;const l=e.proxy,o=i;for(;r;){const c=r.ec;if(c){for(let h=0;h<c.length;h++)if(c[h](t,l,o)===!1)return}r=r.parent}const a=e.appContext.config.errorHandler;if(a){S0(a,null,10,[t,l,o]);return}}T4(t,i,n,s)}function T4(t,e,i,s=!0){console.error(t)}let U1=!1,ss=!1;const Be=[];let d0=0;const qt=[];let Ht=null,ht=0;const Kt=[];let k0=null,ft=0;const br=Promise.resolve();let Hs=null,ns=null;function kr(t){const e=Hs||br;return t?e.then(this?t.bind(this):t):e}function M4(t){let e=d0+1,i=Be.length;for(;e<i;){const s=e+i>>>1;s1(Be[s])<t?e=s+1:i=s}return e}function zr(t){(!Be.length||!Be.includes(t,U1&&t.allowRecurse?d0+1:d0))&&t!==ns&&(t.id==null?Be.push(t):Be.splice(M4(t.id),0,t),_r())}function _r(){!U1&&!ss&&(ss=!0,Hs=br.then(Er))}function F4(t){const e=Be.indexOf(t);e>d0&&Be.splice(e,1)}function Cr(t,e,i,s){q(t)?i.push(...t):(!e||!e.includes(t,t.allowRecurse?s+1:s))&&i.push(t),_r()}function I4(t){Cr(t,Ht,qt,ht)}function j4(t){Cr(t,k0,Kt,ft)}function hi(t,e=null){if(qt.length){for(ns=e,Ht=[...new Set(qt)],qt.length=0,ht=0;ht<Ht.length;ht++)Ht[ht]();Ht=null,ht=0,ns=null,hi(t,e)}}function $r(t){if(hi(),Kt.length){const e=[...new Set(Kt)];if(Kt.length=0,k0){k0.push(...e);return}for(k0=e,k0.sort((i,s)=>s1(i)-s1(s)),ft=0;ft<k0.length;ft++)k0[ft]();k0=null,ft=0}}const s1=t=>t.id==null?1/0:t.id;function Er(t){ss=!1,U1=!0,hi(t),Be.sort((i,s)=>s1(i)-s1(s));const e=Ze;try{for(d0=0;d0<Be.length;d0++){const i=Be[d0];i&&i.active!==!1&&S0(i,null,14)}}finally{d0=0,Be.length=0,$r(),U1=!1,Hs=null,(Be.length||qt.length||Kt.length)&&Er(t)}}function B4(t,e,...i){if(t.isUnmounted)return;const s=t.vnode.props||de;let n=i;const r=e.startsWith("update:"),l=r&&e.slice(7);if(l&&l in s){const h=`${l==="modelValue"?"model":l}Modifiers`,{number:p,trim:d}=s[h]||de;d&&(n=i.map(g=>g.trim())),p&&(n=i.map(sr))}let o,a=s[o=Pi(e)]||s[o=Pi(a0(e))];!a&&r&&(a=s[o=Pi(Pt(e))]),a&&Ge(a,t,6,n);const c=s[o+"Once"];if(c){if(!t.emitted)t.emitted={};else if(t.emitted[o])return;t.emitted[o]=!0,Ge(c,t,6,n)}}function Sr(t,e,i=!1){const s=e.emitsCache,n=s.get(t);if(n!==void 0)return n;const r=t.emits;let l={},o=!1;if(!Y(t)){const a=c=>{const h=Sr(c,e,!0);h&&(o=!0,Pe(l,h))};!i&&e.mixins.length&&e.mixins.forEach(a),t.extends&&a(t.extends),t.mixins&&t.mixins.forEach(a)}return!r&&!o?(s.set(t,null),null):(q(r)?r.forEach(a=>l[a]=null):Pe(l,r),s.set(t,l),l)}function fi(t,e){return!t||!si(e)?!1:(e=e.slice(2).replace(/Once$/,""),se(t,e[0].toLowerCase()+e.slice(1))||se(t,Pt(e))||se(t,e))}let Ve=null,Pr=null;function V1(t){const e=Ve;return Ve=t,Pr=t&&t.type.__scopeId||null,e}function N4(t,e=Ve,i){if(!e||t._n)return t;const s=(...n)=>{s._d&&Rn(-1);const r=V1(e),l=t(...n);return V1(r),s._d&&Rn(1),l};return s._n=!0,s._c=!0,s._d=!0,s}function Di(t){const{type:e,vnode:i,proxy:s,withProxy:n,props:r,propsOptions:[l],slots:o,attrs:a,emit:c,render:h,renderCache:p,data:d,setupState:g,ctx:x,inheritAttrs:A}=t;let C,D;const z=V1(t);try{if(i.shapeFlag&4){const P=n||s;C=l0(h.call(P,P,p,r,g,d,x)),D=a}else{const P=e;C=l0(P.length>1?P(r,{attrs:a,slots:o,emit:c}):P(r,null)),D=e.props?a:L4(a)}}catch(P){Zt.length=0,ci(P,t,1),C=Me(Je)}let O=C;if(D&&A!==!1){const P=Object.keys(D),{shapeFlag:W}=O;P.length&&W&7&&(l&&P.some(Ds)&&(D=H4(D,l)),O=u0(O,D))}return i.dirs&&(O=u0(O),O.dirs=O.dirs?O.dirs.concat(i.dirs):i.dirs),i.transition&&(O.transition=i.transition),C=O,V1(z),C}const L4=t=>{let e;for(const i in t)(i==="class"||i==="style"||si(i))&&((e||(e={}))[i]=t[i]);return e},H4=(t,e)=>{const i={};for(const s in t)(!Ds(s)||!(s.slice(9)in e))&&(i[s]=t[s]);return i};function U4(t,e,i){const{props:s,children:n,component:r}=t,{props:l,children:o,patchFlag:a}=e,c=r.emitsOptions;if(e.dirs||e.transition)return!0;if(i&&a>=0){if(a&1024)return!0;if(a&16)return s?kn(s,l,c):!!l;if(a&8){const h=e.dynamicProps;for(let p=0;p<h.length;p++){const d=h[p];if(l[d]!==s[d]&&!fi(c,d))return!0}}}else return(n||o)&&(!o||!o.$stable)?!0:s===l?!1:s?l?kn(s,l,c):!0:!!l;return!1}function kn(t,e,i){const s=Object.keys(e);if(s.length!==Object.keys(t).length)return!0;for(let n=0;n<s.length;n++){const r=s[n];if(e[r]!==t[r]&&!fi(i,r))return!0}return!1}function V4({vnode:t,parent:e},i){for(;e&&e.subTree===t;)(t=e.vnode).el=i,e=e.parent}const Dr=t=>t.__isSuspense;function G4(t,e){e&&e.pendingBranch?q(t)?e.effects.push(...t):e.effects.push(t):j4(t)}function j1(t,e){if(Ce){let i=Ce.provides;const s=Ce.parent&&Ce.parent.provides;s===i&&(i=Ce.provides=Object.create(s)),i[t]=e}}function P0(t,e,i=!1){const s=Ce||Ve;if(s){const n=s.parent==null?s.vnode.appContext&&s.vnode.appContext.provides:s.parent.provides;if(n&&t in n)return n[t];if(arguments.length>1)return i&&Y(e)?e.call(s.proxy):e}}const zn={};function Xt(t,e,i){return Ar(t,e,i)}function Ar(t,e,{immediate:i,deep:s,flush:n,onTrack:r,onTrigger:l}=de){const o=Ce;let a,c=!1,h=!1;if(Se(t)?(a=()=>t.value,c=is(t)):gt(t)?(a=()=>t,s=!0):q(t)?(h=!0,c=t.some(D=>gt(D)||is(D)),a=()=>t.map(D=>{if(Se(D))return D.value;if(gt(D))return K0(D);if(Y(D))return S0(D,o,2)})):Y(t)?e?a=()=>S0(t,o,2):a=()=>{if(!(o&&o.isUnmounted))return p&&p(),Ge(t,o,3,[d])}:a=Ze,e&&s){const D=a;a=()=>K0(D())}let p,d=D=>{p=C.onStop=()=>{S0(D,o,4)}};if(r1)return d=Ze,e?i&&Ge(e,o,3,[a(),h?[]:void 0,d]):a(),Ze;let g=h?[]:zn;const x=()=>{if(!!C.active)if(e){const D=C.run();(s||c||(h?D.some((z,O)=>e1(z,g[O])):e1(D,g)))&&(p&&p(),Ge(e,o,3,[D,g===zn?void 0:g,d]),g=D)}else C.run()};x.allowRecurse=!!e;let A;n==="sync"?A=x:n==="post"?A=()=>Ee(x,o&&o.suspense):A=()=>I4(x);const C=new Ms(a,A);return e?i?x():g=C.run():n==="post"?Ee(C.run.bind(C),o&&o.suspense):C.run(),()=>{C.stop(),o&&o.scope&&As(o.scope.effects,C)}}function q4(t,e,i){const s=this.proxy,n=we(t)?t.includes(".")?Or(s,t):()=>s[t]:t.bind(s,s);let r;Y(e)?r=e:(r=e.handler,i=e);const l=Ce;xt(this);const o=Ar(n,r.bind(s),i);return l?xt(l):W0(),o}function Or(t,e){const i=e.split(".");return()=>{let s=t;for(let n=0;n<i.length&&s;n++)s=s[i[n]];return s}}function K0(t,e){if(!be(t)||t.__v_skip||(e=e||new Set,e.has(t)))return t;if(e.add(t),Se(t))K0(t.value,e);else if(q(t))for(let i=0;i<t.length;i++)K0(t[i],e);else if(Q2(t)||mt(t))t.forEach(i=>{K0(i,e)});else if(ir(t))for(const i in t)K0(t[i],e);return t}function K4(){const t={isMounted:!1,isLeaving:!1,isUnmounting:!1,leavingVNodes:new Map};return Vs(()=>{t.isMounted=!0}),Gs(()=>{t.isUnmounting=!0}),t}const He=[Function,Array],X4={name:"BaseTransition",props:{mode:String,appear:Boolean,persisted:Boolean,onBeforeEnter:He,onEnter:He,onAfterEnter:He,onEnterCancelled:He,onBeforeLeave:He,onLeave:He,onAfterLeave:He,onLeaveCancelled:He,onBeforeAppear:He,onAppear:He,onAfterAppear:He,onAppearCancelled:He},setup(t,{slots:e}){const i=Qr(),s=K4();let n;return()=>{const r=e.default&&Mr(e.default(),!0);if(!r||!r.length)return;let l=r[0];if(r.length>1){for(const A of r)if(A.type!==Je){l=A;break}}const o=re(t),{mode:a}=o;if(s.isLeaving)return Ai(l);const c=_n(l);if(!c)return Ai(l);const h=rs(c,o,s,i);G1(c,h);const p=i.subTree,d=p&&_n(p);let g=!1;const{getTransitionKey:x}=c.type;if(x){const A=x();n===void 0?n=A:A!==n&&(n=A,g=!0)}if(d&&d.type!==Je&&(!G0(c,d)||g)){const A=rs(d,o,s,i);if(G1(d,A),a==="out-in")return s.isLeaving=!0,A.afterLeave=()=>{s.isLeaving=!1,i.update()},Ai(l);a==="in-out"&&c.type!==Je&&(A.delayLeave=(C,D,z)=>{const O=Tr(s,d);O[String(d.key)]=d,C._leaveCb=()=>{D(),C._leaveCb=void 0,delete h.delayedLeave},h.delayedLeave=z})}return l}}},Rr=X4;function Tr(t,e){const{leavingVNodes:i}=t;let s=i.get(e.type);return s||(s=Object.create(null),i.set(e.type,s)),s}function rs(t,e,i,s){const{appear:n,mode:r,persisted:l=!1,onBeforeEnter:o,onEnter:a,onAfterEnter:c,onEnterCancelled:h,onBeforeLeave:p,onLeave:d,onAfterLeave:g,onLeaveCancelled:x,onBeforeAppear:A,onAppear:C,onAfterAppear:D,onAppearCancelled:z}=e,O=String(t.key),P=Tr(i,t),W=(N,Q)=>{N&&Ge(N,s,9,Q)},J=(N,Q)=>{const ie=Q[1];W(N,Q),q(N)?N.every(le=>le.length<=1)&&ie():N.length<=1&&ie()},K={mode:r,persisted:l,beforeEnter(N){let Q=o;if(!i.isMounted)if(n)Q=A||o;else return;N._leaveCb&&N._leaveCb(!0);const ie=P[O];ie&&G0(t,ie)&&ie.el._leaveCb&&ie.el._leaveCb(),W(Q,[N])},enter(N){let Q=a,ie=c,le=h;if(!i.isMounted)if(n)Q=C||a,ie=D||c,le=z||h;else return;let T=!1;const me=N._enterCb=Ae=>{T||(T=!0,Ae?W(le,[N]):W(ie,[N]),K.delayedLeave&&K.delayedLeave(),N._enterCb=void 0)};Q?J(Q,[N,me]):me()},leave(N,Q){const ie=String(t.key);if(N._enterCb&&N._enterCb(!0),i.isUnmounting)return Q();W(p,[N]);let le=!1;const T=N._leaveCb=me=>{le||(le=!0,Q(),me?W(x,[N]):W(g,[N]),N._leaveCb=void 0,P[ie]===t&&delete P[ie])};P[ie]=t,d?J(d,[N,T]):T()},clone(N){return rs(N,e,i,s)}};return K}function Ai(t){if(di(t))return t=u0(t),t.children=null,t}function _n(t){return di(t)?t.children?t.children[0]:void 0:t}function G1(t,e){t.shapeFlag&6&&t.component?G1(t.component.subTree,e):t.shapeFlag&128?(t.ssContent.transition=e.clone(t.ssContent),t.ssFallback.transition=e.clone(t.ssFallback)):t.transition=e}function Mr(t,e=!1,i){let s=[],n=0;for(let r=0;r<t.length;r++){let l=t[r];const o=i==null?l.key:String(i)+String(l.key!=null?l.key:r);l.type===r0?(l.patchFlag&128&&n++,s=s.concat(Mr(l.children,e,o))):(e||l.type!==Je)&&s.push(o!=null?u0(l,{key:o}):l)}if(n>1)for(let r=0;r<s.length;r++)s[r].patchFlag=-2;return s}function Us(t){return Y(t)?{setup:t,name:t.name}:t}const Wt=t=>!!t.type.__asyncLoader,di=t=>t.type.__isKeepAlive,W4={name:"KeepAlive",__isKeepAlive:!0,props:{include:[String,RegExp,Array],exclude:[String,RegExp,Array],max:[String,Number]},setup(t,{slots:e}){const i=Qr(),s=i.ctx;if(!s.renderer)return()=>{const z=e.default&&e.default();return z&&z.length===1?z[0]:z};const n=new Map,r=new Set;let l=null;const o=i.suspense,{renderer:{p:a,m:c,um:h,o:{createElement:p}}}=s,d=p("div");s.activate=(z,O,P,W,J)=>{const K=z.component;c(z,O,P,0,o),a(K.vnode,z,O,P,K,o,W,z.slotScopeIds,J),Ee(()=>{K.isDeactivated=!1,K.a&&Vt(K.a);const N=z.props&&z.props.onVnodeMounted;N&&Ue(N,K.parent,z)},o)},s.deactivate=z=>{const O=z.component;c(z,d,null,1,o),Ee(()=>{O.da&&Vt(O.da);const P=z.props&&z.props.onVnodeUnmounted;P&&Ue(P,O.parent,z),O.isDeactivated=!0},o)};function g(z){Oi(z),h(z,i,o,!0)}function x(z){n.forEach((O,P)=>{const W=ds(O.type);W&&(!z||!z(W))&&A(P)})}function A(z){const O=n.get(z);!l||O.type!==l.type?g(O):l&&Oi(l),n.delete(z),r.delete(z)}Xt(()=>[t.include,t.exclude],([z,O])=>{z&&x(P=>Ut(z,P)),O&&x(P=>!Ut(O,P))},{flush:"post",deep:!0});let C=null;const D=()=>{C!=null&&n.set(C,Ri(i.subTree))};return Vs(D),Ir(D),Gs(()=>{n.forEach(z=>{const{subTree:O,suspense:P}=i,W=Ri(O);if(z.type===W.type){Oi(W);const J=W.component.da;J&&Ee(J,P);return}g(z)})}),()=>{if(C=null,!e.default)return null;const z=e.default(),O=z[0];if(z.length>1)return l=null,z;if(!X1(O)||!(O.shapeFlag&4)&&!(O.shapeFlag&128))return l=null,O;let P=Ri(O);const W=P.type,J=ds(Wt(P)?P.type.__asyncResolved||{}:W),{include:K,exclude:N,max:Q}=t;if(K&&(!J||!Ut(K,J))||N&&J&&Ut(N,J))return l=P,O;const ie=P.key==null?W:P.key,le=n.get(ie);return P.el&&(P=u0(P),O.shapeFlag&128&&(O.ssContent=P)),C=ie,le?(P.el=le.el,P.component=le.component,P.transition&&G1(P,P.transition),P.shapeFlag|=512,r.delete(ie),r.add(ie)):(r.add(ie),Q&&r.size>parseInt(Q,10)&&A(r.values().next().value)),P.shapeFlag|=256,l=P,Dr(O.type)?O:P}}},Wc=W4;function Ut(t,e){return q(t)?t.some(i=>Ut(i,e)):we(t)?t.split(",").includes(e):t.test?t.test(e):!1}function Y4(t,e){Fr(t,"a",e)}function Z4(t,e){Fr(t,"da",e)}function Fr(t,e,i=Ce){const s=t.__wdc||(t.__wdc=()=>{let n=i;for(;n;){if(n.isDeactivated)return;n=n.parent}return t()});if(pi(e,s,i),i){let n=i.parent;for(;n&&n.parent;)di(n.parent.vnode)&&J4(s,e,i,n),n=n.parent}}function J4(t,e,i,s){const n=pi(e,t,s,!0);jr(()=>{As(s[e],n)},i)}function Oi(t){let e=t.shapeFlag;e&256&&(e-=256),e&512&&(e-=512),t.shapeFlag=e}function Ri(t){return t.shapeFlag&128?t.ssContent:t}function pi(t,e,i=Ce,s=!1){if(i){const n=i[t]||(i[t]=[]),r=e.__weh||(e.__weh=(...l)=>{if(i.isUnmounted)return;Dt(),xt(i);const o=Ge(e,i,t,l);return W0(),At(),o});return s?n.unshift(r):n.push(r),r}}const m0=t=>(e,i=Ce)=>(!r1||t==="sp")&&pi(t,e,i),Q4=m0("bm"),Vs=m0("m"),eo=m0("bu"),Ir=m0("u"),Gs=m0("bum"),jr=m0("um"),to=m0("sp"),io=m0("rtg"),so=m0("rtc");function no(t,e=Ce){pi("ec",t,e)}function Yc(t,e){const i=Ve;if(i===null)return t;const s=mi(i)||i.proxy,n=t.dirs||(t.dirs=[]);for(let r=0;r<e.length;r++){let[l,o,a,c=de]=e[r];Y(l)&&(l={mounted:l,updated:l}),l.deep&&K0(o),n.push({dir:l,instance:s,value:o,oldValue:void 0,arg:a,modifiers:c})}return t}function B0(t,e,i,s){const n=t.dirs,r=e&&e.dirs;for(let l=0;l<n.length;l++){const o=n[l];r&&(o.oldValue=r[l].value);let a=o.dir[s];a&&(Dt(),Ge(a,i,8,[t.el,o,t,e]),At())}}const Br="components";function ro(t,e){return oo(Br,t,!0,e)||t}const lo=Symbol();function oo(t,e,i=!0,s=!1){const n=Ve||Ce;if(n){const r=n.type;if(t===Br){const o=ds(r);if(o&&(o===e||o===a0(e)||o===li(a0(e))))return r}const l=Cn(n[t]||r[t],e)||Cn(n.appContext[t],e);return!l&&s?r:l}}function Cn(t,e){return t&&(t[e]||t[a0(e)]||t[li(a0(e))])}function Zc(t,e,i,s){let n;const r=i&&i[s];if(q(t)||we(t)){n=new Array(t.length);for(let l=0,o=t.length;l<o;l++)n[l]=e(t[l],l,void 0,r&&r[l])}else if(typeof t=="number"){n=new Array(t);for(let l=0;l<t;l++)n[l]=e(l+1,l,void 0,r&&r[l])}else if(be(t))if(t[Symbol.iterator])n=Array.from(t,(l,o)=>e(l,o,void 0,r&&r[o]));else{const l=Object.keys(t);n=new Array(l.length);for(let o=0,a=l.length;o<a;o++){const c=l[o];n[o]=e(t[c],c,o,r&&r[o])}}else n=[];return i&&(i[s]=n),n}const ls=t=>t?el(t)?mi(t)||t.proxy:ls(t.parent):null,q1=Pe(Object.create(null),{$:t=>t,$el:t=>t.vnode.el,$data:t=>t.data,$props:t=>t.props,$attrs:t=>t.attrs,$slots:t=>t.slots,$refs:t=>t.refs,$parent:t=>ls(t.parent),$root:t=>ls(t.root),$emit:t=>t.emit,$options:t=>Lr(t),$forceUpdate:t=>t.f||(t.f=()=>zr(t.update)),$nextTick:t=>t.n||(t.n=kr.bind(t.proxy)),$watch:t=>q4.bind(t)}),ao={get({_:t},e){const{ctx:i,setupState:s,data:n,props:r,accessCache:l,type:o,appContext:a}=t;let c;if(e[0]!=="$"){const g=l[e];if(g!==void 0)switch(g){case 1:return s[e];case 2:return n[e];case 4:return i[e];case 3:return r[e]}else{if(s!==de&&se(s,e))return l[e]=1,s[e];if(n!==de&&se(n,e))return l[e]=2,n[e];if((c=t.propsOptions[0])&&se(c,e))return l[e]=3,r[e];if(i!==de&&se(i,e))return l[e]=4,i[e];os&&(l[e]=0)}}const h=q1[e];let p,d;if(h)return e==="$attrs"&&Ne(t,"get",e),h(t);if((p=o.__cssModules)&&(p=p[e]))return p;if(i!==de&&se(i,e))return l[e]=4,i[e];if(d=a.config.globalProperties,se(d,e))return d[e]},set({_:t},e,i){const{data:s,setupState:n,ctx:r}=t;return n!==de&&se(n,e)?(n[e]=i,!0):s!==de&&se(s,e)?(s[e]=i,!0):se(t.props,e)||e[0]==="$"&&e.slice(1)in t?!1:(r[e]=i,!0)},has({_:{data:t,setupState:e,accessCache:i,ctx:s,appContext:n,propsOptions:r}},l){let o;return!!i[l]||t!==de&&se(t,l)||e!==de&&se(e,l)||(o=r[0])&&se(o,l)||se(s,l)||se(q1,l)||se(n.config.globalProperties,l)},defineProperty(t,e,i){return i.get!=null?t._.accessCache[e]=0:se(i,"value")&&this.set(t,e,i.value,null),Reflect.defineProperty(t,e,i)}};let os=!0;function co(t){const e=Lr(t),i=t.proxy,s=t.ctx;os=!1,e.beforeCreate&&$n(e.beforeCreate,t,"bc");const{data:n,computed:r,methods:l,watch:o,provide:a,inject:c,created:h,beforeMount:p,mounted:d,beforeUpdate:g,updated:x,activated:A,deactivated:C,beforeDestroy:D,beforeUnmount:z,destroyed:O,unmounted:P,render:W,renderTracked:J,renderTriggered:K,errorCaptured:N,serverPrefetch:Q,expose:ie,inheritAttrs:le,components:T,directives:me,filters:Ae}=e;if(c&&ho(c,s,null,t.appContext.config.unwrapInjectedRef),l)for(const pe in l){const oe=l[pe];Y(oe)&&(s[pe]=oe.bind(i))}if(n){const pe=n.call(i,i);be(pe)&&(t.data=Q0(pe))}if(os=!0,r)for(const pe in r){const oe=r[pe],Ie=Y(oe)?oe.bind(i,i):Y(oe.get)?oe.get.bind(i,i):Ze,lt=!Y(oe)&&Y(oe.set)?oe.set.bind(i):Ze,f0=o0({get:Ie,set:lt});Object.defineProperty(s,pe,{enumerable:!0,configurable:!0,get:()=>f0.value,set:e0=>f0.value=e0})}if(o)for(const pe in o)Nr(o[pe],s,i,pe);if(a){const pe=Y(a)?a.call(i):a;Reflect.ownKeys(pe).forEach(oe=>{j1(oe,pe[oe])})}h&&$n(h,t,"c");function ye(pe,oe){q(oe)?oe.forEach(Ie=>pe(Ie.bind(i))):oe&&pe(oe.bind(i))}if(ye(Q4,p),ye(Vs,d),ye(eo,g),ye(Ir,x),ye(Y4,A),ye(Z4,C),ye(no,N),ye(so,J),ye(io,K),ye(Gs,z),ye(jr,P),ye(to,Q),q(ie))if(ie.length){const pe=t.exposed||(t.exposed={});ie.forEach(oe=>{Object.defineProperty(pe,oe,{get:()=>i[oe],set:Ie=>i[oe]=Ie})})}else t.exposed||(t.exposed={});W&&t.render===Ze&&(t.render=W),le!=null&&(t.inheritAttrs=le),T&&(t.components=T),me&&(t.directives=me)}function ho(t,e,i=Ze,s=!1){q(t)&&(t=as(t));for(const n in t){const r=t[n];let l;be(r)?"default"in r?l=P0(r.from||n,r.default,!0):l=P0(r.from||n):l=P0(r),Se(l)&&s?Object.defineProperty(e,n,{enumerable:!0,configurable:!0,get:()=>l.value,set:o=>l.value=o}):e[n]=l}}function $n(t,e,i){Ge(q(t)?t.map(s=>s.bind(e.proxy)):t.bind(e.proxy),e,i)}function Nr(t,e,i,s){const n=s.includes(".")?Or(i,s):()=>i[s];if(we(t)){const r=e[t];Y(r)&&Xt(n,r)}else if(Y(t))Xt(n,t.bind(i));else if(be(t))if(q(t))t.forEach(r=>Nr(r,e,i,s));else{const r=Y(t.handler)?t.handler.bind(i):e[t.handler];Y(r)&&Xt(n,r,t)}}function Lr(t){const e=t.type,{mixins:i,extends:s}=e,{mixins:n,optionsCache:r,config:{optionMergeStrategies:l}}=t.appContext,o=r.get(e);let a;return o?a=o:!n.length&&!i&&!s?a=e:(a={},n.length&&n.forEach(c=>K1(a,c,l,!0)),K1(a,e,l)),r.set(e,a),a}function K1(t,e,i,s=!1){const{mixins:n,extends:r}=e;r&&K1(t,r,i,!0),n&&n.forEach(l=>K1(t,l,i,!0));for(const l in e)if(!(s&&l==="expose")){const o=fo[l]||i&&i[l];t[l]=o?o(t[l],e[l]):e[l]}return t}const fo={data:En,props:V0,emits:V0,methods:V0,computed:V0,beforeCreate:Re,created:Re,beforeMount:Re,mounted:Re,beforeUpdate:Re,updated:Re,beforeDestroy:Re,beforeUnmount:Re,destroyed:Re,unmounted:Re,activated:Re,deactivated:Re,errorCaptured:Re,serverPrefetch:Re,components:V0,directives:V0,watch:uo,provide:En,inject:po};function En(t,e){return e?t?function(){return Pe(Y(t)?t.call(this,this):t,Y(e)?e.call(this,this):e)}:e:t}function po(t,e){return V0(as(t),as(e))}function as(t){if(q(t)){const e={};for(let i=0;i<t.length;i++)e[t[i]]=t[i];return e}return t}function Re(t,e){return t?[...new Set([].concat(t,e))]:e}function V0(t,e){return t?Pe(Pe(Object.create(null),t),e):e}function uo(t,e){if(!t)return e;if(!e)return t;const i=Pe(Object.create(null),t);for(const s in e)i[s]=Re(t[s],e[s]);return i}function mo(t,e,i,s=!1){const n={},r={};H1(r,ui,1),t.propsDefaults=Object.create(null),Hr(t,e,n,r);for(const l in t.propsOptions[0])l in n||(n[l]=void 0);i?t.props=s?n:S4(n):t.type.props?t.props=n:t.props=r,t.attrs=r}function go(t,e,i,s){const{props:n,attrs:r,vnode:{patchFlag:l}}=t,o=re(n),[a]=t.propsOptions;let c=!1;if((s||l>0)&&!(l&16)){if(l&8){const h=t.vnode.dynamicProps;for(let p=0;p<h.length;p++){let d=h[p];if(fi(t.emitsOptions,d))continue;const g=e[d];if(a)if(se(r,d))g!==r[d]&&(r[d]=g,c=!0);else{const x=a0(d);n[x]=cs(a,o,x,g,t,!1)}else g!==r[d]&&(r[d]=g,c=!0)}}}else{Hr(t,e,n,r)&&(c=!0);let h;for(const p in o)(!e||!se(e,p)&&((h=Pt(p))===p||!se(e,h)))&&(a?i&&(i[p]!==void 0||i[h]!==void 0)&&(n[p]=cs(a,o,p,void 0,t,!0)):delete n[p]);if(r!==o)for(const p in r)(!e||!se(e,p)&&!0)&&(delete r[p],c=!0)}c&&p0(t,"set","$attrs")}function Hr(t,e,i,s){const[n,r]=t.propsOptions;let l=!1,o;if(e)for(let a in e){if(I1(a))continue;const c=e[a];let h;n&&se(n,h=a0(a))?!r||!r.includes(h)?i[h]=c:(o||(o={}))[h]=c:fi(t.emitsOptions,a)||(!(a in s)||c!==s[a])&&(s[a]=c,l=!0)}if(r){const a=re(i),c=o||de;for(let h=0;h<r.length;h++){const p=r[h];i[p]=cs(n,a,p,c[p],t,!se(c,p))}}return l}function cs(t,e,i,s,n,r){const l=t[i];if(l!=null){const o=se(l,"default");if(o&&s===void 0){const a=l.default;if(l.type!==Function&&Y(a)){const{propsDefaults:c}=n;i in c?s=c[i]:(xt(n),s=c[i]=a.call(null,e),W0())}else s=a}l[0]&&(r&&!o?s=!1:l[1]&&(s===""||s===Pt(i))&&(s=!0))}return s}function Ur(t,e,i=!1){const s=e.propsCache,n=s.get(t);if(n)return n;const r=t.props,l={},o=[];let a=!1;if(!Y(t)){const h=p=>{a=!0;const[d,g]=Ur(p,e,!0);Pe(l,d),g&&o.push(...g)};!i&&e.mixins.length&&e.mixins.forEach(h),t.extends&&h(t.extends),t.mixins&&t.mixins.forEach(h)}if(!r&&!a)return s.set(t,ut),ut;if(q(r))for(let h=0;h<r.length;h++){const p=a0(r[h]);Sn(p)&&(l[p]=de)}else if(r)for(const h in r){const p=a0(h);if(Sn(p)){const d=r[h],g=l[p]=q(d)||Y(d)?{type:d}:d;if(g){const x=An(Boolean,g.type),A=An(String,g.type);g[0]=x>-1,g[1]=A<0||x<A,(x>-1||se(g,"default"))&&o.push(p)}}}const c=[l,o];return s.set(t,c),c}function Sn(t){return t[0]!=="$"}function Pn(t){const e=t&&t.toString().match(/^\s*function (\w+)/);return e?e[1]:t===null?"null":""}function Dn(t,e){return Pn(t)===Pn(e)}function An(t,e){return q(e)?e.findIndex(i=>Dn(i,t)):Y(e)&&Dn(e,t)?0:-1}const Vr=t=>t[0]==="_"||t==="$stable",qs=t=>q(t)?t.map(l0):[l0(t)],yo=(t,e,i)=>{if(e._n)return e;const s=N4((...n)=>qs(e(...n)),i);return s._c=!1,s},Gr=(t,e,i)=>{const s=t._ctx;for(const n in t){if(Vr(n))continue;const r=t[n];if(Y(r))e[n]=yo(n,r,s);else if(r!=null){const l=qs(r);e[n]=()=>l}}},qr=(t,e)=>{const i=qs(e);t.slots.default=()=>i},xo=(t,e)=>{if(t.vnode.shapeFlag&32){const i=e._;i?(t.slots=re(e),H1(e,"_",i)):Gr(e,t.slots={})}else t.slots={},e&&qr(t,e);H1(t.slots,ui,1)},vo=(t,e,i)=>{const{vnode:s,slots:n}=t;let r=!0,l=de;if(s.shapeFlag&32){const o=e._;o?i&&o===1?r=!1:(Pe(n,e),!i&&o===1&&delete n._):(r=!e.$stable,Gr(e,n)),l=e}else e&&(qr(t,e),l={default:1});if(r)for(const o in n)!Vr(o)&&!(o in l)&&delete n[o]};function Kr(){return{app:null,config:{isNativeTag:Xl,performance:!1,globalProperties:{},optionMergeStrategies:{},errorHandler:void 0,warnHandler:void 0,compilerOptions:{}},mixins:[],components:{},directives:{},provides:Object.create(null),optionsCache:new WeakMap,propsCache:new WeakMap,emitsCache:new WeakMap}}let wo=0;function bo(t,e){return function(s,n=null){Y(s)||(s=Object.assign({},s)),n!=null&&!be(n)&&(n=null);const r=Kr(),l=new Set;let o=!1;const a=r.app={_uid:wo++,_component:s,_props:n,_container:null,_context:r,_instance:null,version:Lo,get config(){return r.config},set config(c){},use(c,...h){return l.has(c)||(c&&Y(c.install)?(l.add(c),c.install(a,...h)):Y(c)&&(l.add(c),c(a,...h))),a},mixin(c){return r.mixins.includes(c)||r.mixins.push(c),a},component(c,h){return h?(r.components[c]=h,a):r.components[c]},directive(c,h){return h?(r.directives[c]=h,a):r.directives[c]},mount(c,h,p){if(!o){const d=Me(s,n);return d.appContext=r,h&&e?e(d,c):t(d,c,p),o=!0,a._container=c,c.__vue_app__=a,mi(d.component)||d.component.proxy}},unmount(){o&&(t(null,a._container),delete a._container.__vue_app__)},provide(c,h){return r.provides[c]=h,a}};return a}}function hs(t,e,i,s,n=!1){if(q(t)){t.forEach((d,g)=>hs(d,e&&(q(e)?e[g]:e),i,s,n));return}if(Wt(s)&&!n)return;const r=s.shapeFlag&4?mi(s.component)||s.component.proxy:s.el,l=n?null:r,{i:o,r:a}=t,c=e&&e.r,h=o.refs===de?o.refs={}:o.refs,p=o.setupState;if(c!=null&&c!==a&&(we(c)?(h[c]=null,se(p,c)&&(p[c]=null)):Se(c)&&(c.value=null)),Y(a))S0(a,o,12,[l,h]);else{const d=we(a),g=Se(a);if(d||g){const x=()=>{if(t.f){const A=d?h[a]:a.value;n?q(A)&&As(A,r):q(A)?A.includes(r)||A.push(r):d?(h[a]=[r],se(p,a)&&(p[a]=h[a])):(a.value=[r],t.k&&(h[t.k]=a.value))}else d?(h[a]=l,se(p,a)&&(p[a]=l)):Se(a)&&(a.value=l,t.k&&(h[t.k]=l))};l?(x.id=-1,Ee(x,i)):x()}}}const Ee=G4;function ko(t){return zo(t)}function zo(t,e){const i=e4();i.__VUE__=!0;const{insert:s,remove:n,patchProp:r,createElement:l,createText:o,createComment:a,setText:c,setElementText:h,parentNode:p,nextSibling:d,setScopeId:g=Ze,cloneNode:x,insertStaticContent:A}=t,C=(f,u,m,b=null,w=null,$=null,R=!1,_=null,E=!!u.dynamicChildren)=>{if(f===u)return;f&&!G0(f,u)&&(b=I(f),Le(f,w,$,!0),f=null),u.patchFlag===-2&&(E=!1,u.dynamicChildren=null);const{type:k,ref:j,shapeFlag:M}=u;switch(k){case Xs:D(f,u,m,b);break;case Je:z(f,u,m,b);break;case B1:f==null&&O(u,m,b,R);break;case r0:me(f,u,m,b,w,$,R,_,E);break;default:M&1?J(f,u,m,b,w,$,R,_,E):M&6?Ae(f,u,m,b,w,$,R,_,E):(M&64||M&128)&&k.process(f,u,m,b,w,$,R,_,E,ue)}j!=null&&w&&hs(j,f&&f.ref,$,u||f,!u)},D=(f,u,m,b)=>{if(f==null)s(u.el=o(u.children),m,b);else{const w=u.el=f.el;u.children!==f.children&&c(w,u.children)}},z=(f,u,m,b)=>{f==null?s(u.el=a(u.children||""),m,b):u.el=f.el},O=(f,u,m,b)=>{[f.el,f.anchor]=A(f.children,u,m,b,f.el,f.anchor)},P=({el:f,anchor:u},m,b)=>{let w;for(;f&&f!==u;)w=d(f),s(f,m,b),f=w;s(u,m,b)},W=({el:f,anchor:u})=>{let m;for(;f&&f!==u;)m=d(f),n(f),f=m;n(u)},J=(f,u,m,b,w,$,R,_,E)=>{R=R||u.type==="svg",f==null?K(u,m,b,w,$,R,_,E):ie(f,u,w,$,R,_,E)},K=(f,u,m,b,w,$,R,_)=>{let E,k;const{type:j,props:M,shapeFlag:B,transition:X,patchFlag:ne,dirs:ce}=f;if(f.el&&x!==void 0&&ne===-1)E=f.el=x(f.el);else{if(E=f.el=l(f.type,$,M&&M.is,M),B&8?h(E,f.children):B&16&&Q(f.children,E,null,b,w,$&&j!=="foreignObject",R,_),ce&&B0(f,null,b,"created"),M){for(const ge in M)ge!=="value"&&!I1(ge)&&r(E,ge,null,M[ge],$,f.children,b,w,S);"value"in M&&r(E,"value",null,M.value),(k=M.onVnodeBeforeMount)&&Ue(k,b,f)}N(E,f,f.scopeId,R,b)}ce&&B0(f,null,b,"beforeMount");const he=(!w||w&&!w.pendingBranch)&&X&&!X.persisted;he&&X.beforeEnter(E),s(E,u,m),((k=M&&M.onVnodeMounted)||he||ce)&&Ee(()=>{k&&Ue(k,b,f),he&&X.enter(E),ce&&B0(f,null,b,"mounted")},w)},N=(f,u,m,b,w)=>{if(m&&g(f,m),b)for(let $=0;$<b.length;$++)g(f,b[$]);if(w){let $=w.subTree;if(u===$){const R=w.vnode;N(f,R,R.scopeId,R.slotScopeIds,w.parent)}}},Q=(f,u,m,b,w,$,R,_,E=0)=>{for(let k=E;k<f.length;k++){const j=f[k]=_?z0(f[k]):l0(f[k]);C(null,j,u,m,b,w,$,R,_)}},ie=(f,u,m,b,w,$,R)=>{const _=u.el=f.el;let{patchFlag:E,dynamicChildren:k,dirs:j}=u;E|=f.patchFlag&16;const M=f.props||de,B=u.props||de;let X;m&&N0(m,!1),(X=B.onVnodeBeforeUpdate)&&Ue(X,m,u,f),j&&B0(u,f,m,"beforeUpdate"),m&&N0(m,!0);const ne=w&&u.type!=="foreignObject";if(k?le(f.dynamicChildren,k,_,m,b,ne,$):R||Ie(f,u,_,null,m,b,ne,$,!1),E>0){if(E&16)T(_,u,M,B,m,b,w);else if(E&2&&M.class!==B.class&&r(_,"class",null,B.class,w),E&4&&r(_,"style",M.style,B.style,w),E&8){const ce=u.dynamicProps;for(let he=0;he<ce.length;he++){const ge=ce[he],Ke=M[ge],ot=B[ge];(ot!==Ke||ge==="value")&&r(_,ge,Ke,ot,w,f.children,m,b,S)}}E&1&&f.children!==u.children&&h(_,u.children)}else!R&&k==null&&T(_,u,M,B,m,b,w);((X=B.onVnodeUpdated)||j)&&Ee(()=>{X&&Ue(X,m,u,f),j&&B0(u,f,m,"updated")},b)},le=(f,u,m,b,w,$,R)=>{for(let _=0;_<u.length;_++){const E=f[_],k=u[_],j=E.el&&(E.type===r0||!G0(E,k)||E.shapeFlag&70)?p(E.el):m;C(E,k,j,null,b,w,$,R,!0)}},T=(f,u,m,b,w,$,R)=>{if(m!==b){for(const _ in b){if(I1(_))continue;const E=b[_],k=m[_];E!==k&&_!=="value"&&r(f,_,k,E,R,u.children,w,$,S)}if(m!==de)for(const _ in m)!I1(_)&&!(_ in b)&&r(f,_,m[_],null,R,u.children,w,$,S);"value"in b&&r(f,"value",m.value,b.value)}},me=(f,u,m,b,w,$,R,_,E)=>{const k=u.el=f?f.el:o(""),j=u.anchor=f?f.anchor:o("");let{patchFlag:M,dynamicChildren:B,slotScopeIds:X}=u;X&&(_=_?_.concat(X):X),f==null?(s(k,m,b),s(j,m,b),Q(u.children,m,j,w,$,R,_,E)):M>0&&M&64&&B&&f.dynamicChildren?(le(f.dynamicChildren,B,m,w,$,R,_),(u.key!=null||w&&u===w.subTree)&&Ks(f,u,!0)):Ie(f,u,m,j,w,$,R,_,E)},Ae=(f,u,m,b,w,$,R,_,E)=>{u.slotScopeIds=_,f==null?u.shapeFlag&512?w.ctx.activate(u,m,b,R,E):h0(u,m,b,w,$,R,E):ye(f,u,E)},h0=(f,u,m,b,w,$,R)=>{const _=f.component=Mo(f,b,w);if(di(f)&&(_.ctx.renderer=ue),Fo(_),_.asyncDep){if(w&&w.registerDep(_,pe),!f.el){const E=_.subTree=Me(Je);z(null,E,u,m)}return}pe(_,f,u,m,w,$,R)},ye=(f,u,m)=>{const b=u.component=f.component;if(U4(f,u,m))if(b.asyncDep&&!b.asyncResolved){oe(b,u,m);return}else b.next=u,F4(b.update),b.update();else u.el=f.el,b.vnode=u},pe=(f,u,m,b,w,$,R)=>{const _=()=>{if(f.isMounted){let{next:j,bu:M,u:B,parent:X,vnode:ne}=f,ce=j,he;N0(f,!1),j?(j.el=ne.el,oe(f,j,R)):j=ne,M&&Vt(M),(he=j.props&&j.props.onVnodeBeforeUpdate)&&Ue(he,X,j,ne),N0(f,!0);const ge=Di(f),Ke=f.subTree;f.subTree=ge,C(Ke,ge,p(Ke.el),I(Ke),f,w,$),j.el=ge.el,ce===null&&V4(f,ge.el),B&&Ee(B,w),(he=j.props&&j.props.onVnodeUpdated)&&Ee(()=>Ue(he,X,j,ne),w)}else{let j;const{el:M,props:B}=u,{bm:X,m:ne,parent:ce}=f,he=Wt(u);if(N0(f,!1),X&&Vt(X),!he&&(j=B&&B.onVnodeBeforeMount)&&Ue(j,ce,u),N0(f,!0),M&&Z){const ge=()=>{f.subTree=Di(f),Z(M,f.subTree,f,w,null)};he?u.type.__asyncLoader().then(()=>!f.isUnmounted&&ge()):ge()}else{const ge=f.subTree=Di(f);C(null,ge,m,b,f,w,$),u.el=ge.el}if(ne&&Ee(ne,w),!he&&(j=B&&B.onVnodeMounted)){const ge=u;Ee(()=>Ue(j,ce,ge),w)}(u.shapeFlag&256||ce&&Wt(ce.vnode)&&ce.vnode.shapeFlag&256)&&f.a&&Ee(f.a,w),f.isMounted=!0,u=m=b=null}},E=f.effect=new Ms(_,()=>zr(k),f.scope),k=f.update=()=>E.run();k.id=f.uid,N0(f,!0),k()},oe=(f,u,m)=>{u.component=f;const b=f.vnode.props;f.vnode=u,f.next=null,go(f,u.props,b,m),vo(f,u.children,m),Dt(),hi(void 0,f.update),At()},Ie=(f,u,m,b,w,$,R,_,E=!1)=>{const k=f&&f.children,j=f?f.shapeFlag:0,M=u.children,{patchFlag:B,shapeFlag:X}=u;if(B>0){if(B&128){f0(k,M,m,b,w,$,R,_,E);return}else if(B&256){lt(k,M,m,b,w,$,R,_,E);return}}X&8?(j&16&&S(k,w,$),M!==k&&h(m,M)):j&16?X&16?f0(k,M,m,b,w,$,R,_,E):S(k,w,$,!0):(j&8&&h(m,""),X&16&&Q(M,m,b,w,$,R,_,E))},lt=(f,u,m,b,w,$,R,_,E)=>{f=f||ut,u=u||ut;const k=f.length,j=u.length,M=Math.min(k,j);let B;for(B=0;B<M;B++){const X=u[B]=E?z0(u[B]):l0(u[B]);C(f[B],X,m,null,w,$,R,_,E)}k>j?S(f,w,$,!0,!1,M):Q(u,m,b,w,$,R,_,E,M)},f0=(f,u,m,b,w,$,R,_,E)=>{let k=0;const j=u.length;let M=f.length-1,B=j-1;for(;k<=M&&k<=B;){const X=f[k],ne=u[k]=E?z0(u[k]):l0(u[k]);if(G0(X,ne))C(X,ne,m,null,w,$,R,_,E);else break;k++}for(;k<=M&&k<=B;){const X=f[M],ne=u[B]=E?z0(u[B]):l0(u[B]);if(G0(X,ne))C(X,ne,m,null,w,$,R,_,E);else break;M--,B--}if(k>M){if(k<=B){const X=B+1,ne=X<j?u[X].el:b;for(;k<=B;)C(null,u[k]=E?z0(u[k]):l0(u[k]),m,ne,w,$,R,_,E),k++}}else if(k>B)for(;k<=M;)Le(f[k],w,$,!0),k++;else{const X=k,ne=k,ce=new Map;for(k=ne;k<=B;k++){const je=u[k]=E?z0(u[k]):l0(u[k]);je.key!=null&&ce.set(je.key,k)}let he,ge=0;const Ke=B-ne+1;let ot=!1,cn=0;const Mt=new Array(Ke);for(k=0;k<Ke;k++)Mt[k]=0;for(k=X;k<=M;k++){const je=f[k];if(ge>=Ke){Le(je,w,$,!0);continue}let t0;if(je.key!=null)t0=ce.get(je.key);else for(he=ne;he<=B;he++)if(Mt[he-ne]===0&&G0(je,u[he])){t0=he;break}t0===void 0?Le(je,w,$,!0):(Mt[t0-ne]=k+1,t0>=cn?cn=t0:ot=!0,C(je,u[t0],m,null,w,$,R,_,E),ge++)}const hn=ot?_o(Mt):ut;for(he=hn.length-1,k=Ke-1;k>=0;k--){const je=ne+k,t0=u[je],fn=je+1<j?u[je+1].el:b;Mt[k]===0?C(null,t0,m,fn,w,$,R,_,E):ot&&(he<0||k!==hn[he]?e0(t0,m,fn,2):he--)}}},e0=(f,u,m,b,w=null)=>{const{el:$,type:R,transition:_,children:E,shapeFlag:k}=f;if(k&6){e0(f.component.subTree,u,m,b);return}if(k&128){f.suspense.move(u,m,b);return}if(k&64){R.move(f,u,m,ue);return}if(R===r0){s($,u,m);for(let M=0;M<E.length;M++)e0(E[M],u,m,b);s(f.anchor,u,m);return}if(R===B1){P(f,u,m);return}if(b!==2&&k&1&&_)if(b===0)_.beforeEnter($),s($,u,m),Ee(()=>_.enter($),w);else{const{leave:M,delayLeave:B,afterLeave:X}=_,ne=()=>s($,u,m),ce=()=>{M($,()=>{ne(),X&&X()})};B?B($,ne,ce):ce()}else s($,u,m)},Le=(f,u,m,b=!1,w=!1)=>{const{type:$,props:R,ref:_,children:E,dynamicChildren:k,shapeFlag:j,patchFlag:M,dirs:B}=f;if(_!=null&&hs(_,null,m,f,!0),j&256){u.ctx.deactivate(f);return}const X=j&1&&B,ne=!Wt(f);let ce;if(ne&&(ce=R&&R.onVnodeBeforeUnmount)&&Ue(ce,u,f),j&6)F(f.component,m,b);else{if(j&128){f.suspense.unmount(m,b);return}X&&B0(f,null,u,"beforeUnmount"),j&64?f.type.remove(f,u,m,w,ue,b):k&&($!==r0||M>0&&M&64)?S(k,u,m,!1,!0):($===r0&&M&384||!w&&j&16)&&S(E,u,m),b&&Si(f)}(ne&&(ce=R&&R.onVnodeUnmounted)||X)&&Ee(()=>{ce&&Ue(ce,u,f),X&&B0(f,null,u,"unmounted")},m)},Si=f=>{const{type:u,el:m,anchor:b,transition:w}=f;if(u===r0){v(m,b);return}if(u===B1){W(f);return}const $=()=>{n(m),w&&!w.persisted&&w.afterLeave&&w.afterLeave()};if(f.shapeFlag&1&&w&&!w.persisted){const{leave:R,delayLeave:_}=w,E=()=>R(m,$);_?_(f.el,$,E):E()}else $()},v=(f,u)=>{let m;for(;f!==u;)m=d(f),n(f),f=m;n(u)},F=(f,u,m)=>{const{bum:b,scope:w,update:$,subTree:R,um:_}=f;b&&Vt(b),w.stop(),$&&($.active=!1,Le(R,f,u,m)),_&&Ee(_,u),Ee(()=>{f.isUnmounted=!0},u),u&&u.pendingBranch&&!u.isUnmounted&&f.asyncDep&&!f.asyncResolved&&f.suspenseId===u.pendingId&&(u.deps--,u.deps===0&&u.resolve())},S=(f,u,m,b=!1,w=!1,$=0)=>{for(let R=$;R<f.length;R++)Le(f[R],u,m,b,w)},I=f=>f.shapeFlag&6?I(f.component.subTree):f.shapeFlag&128?f.suspense.next():d(f.anchor||f.el),ae=(f,u,m)=>{f==null?u._vnode&&Le(u._vnode,null,null,!0):C(u._vnode||null,f,u,null,null,null,m),$r(),u._vnode=f},ue={p:C,um:Le,m:e0,r:Si,mt:h0,mc:Q,pc:Ie,pbc:le,n:I,o:t};let ee,Z;return e&&([ee,Z]=e(ue)),{render:ae,hydrate:ee,createApp:bo(ae,ee)}}function N0({effect:t,update:e},i){t.allowRecurse=e.allowRecurse=i}function Ks(t,e,i=!1){const s=t.children,n=e.children;if(q(s)&&q(n))for(let r=0;r<s.length;r++){const l=s[r];let o=n[r];o.shapeFlag&1&&!o.dynamicChildren&&((o.patchFlag<=0||o.patchFlag===32)&&(o=n[r]=z0(n[r]),o.el=l.el),i||Ks(l,o))}}function _o(t){const e=t.slice(),i=[0];let s,n,r,l,o;const a=t.length;for(s=0;s<a;s++){const c=t[s];if(c!==0){if(n=i[i.length-1],t[n]<c){e[s]=n,i.push(s);continue}for(r=0,l=i.length-1;r<l;)o=r+l>>1,t[i[o]]<c?r=o+1:l=o;c<t[i[r]]&&(r>0&&(e[s]=i[r-1]),i[r]=s)}}for(r=i.length,l=i[r-1];r-- >0;)i[r]=l,l=e[l];return i}const Co=t=>t.__isTeleport,Yt=t=>t&&(t.disabled||t.disabled===""),On=t=>typeof SVGElement!="undefined"&&t instanceof SVGElement,fs=(t,e)=>{const i=t&&t.to;return we(i)?e?e(i):null:i},$o={__isTeleport:!0,process(t,e,i,s,n,r,l,o,a,c){const{mc:h,pc:p,pbc:d,o:{insert:g,querySelector:x,createText:A,createComment:C}}=c,D=Yt(e.props);let{shapeFlag:z,children:O,dynamicChildren:P}=e;if(t==null){const W=e.el=A(""),J=e.anchor=A("");g(W,i,s),g(J,i,s);const K=e.target=fs(e.props,x),N=e.targetAnchor=A("");K&&(g(N,K),l=l||On(K));const Q=(ie,le)=>{z&16&&h(O,ie,le,n,r,l,o,a)};D?Q(i,J):K&&Q(K,N)}else{e.el=t.el;const W=e.anchor=t.anchor,J=e.target=t.target,K=e.targetAnchor=t.targetAnchor,N=Yt(t.props),Q=N?i:J,ie=N?W:K;if(l=l||On(J),P?(d(t.dynamicChildren,P,Q,n,r,l,o),Ks(t,e,!0)):a||p(t,e,Q,ie,n,r,l,o,!1),D)N||P1(e,i,W,c,1);else if((e.props&&e.props.to)!==(t.props&&t.props.to)){const le=e.target=fs(e.props,x);le&&P1(e,le,null,c,0)}else N&&P1(e,J,K,c,1)}},remove(t,e,i,s,{um:n,o:{remove:r}},l){const{shapeFlag:o,children:a,anchor:c,targetAnchor:h,target:p,props:d}=t;if(p&&r(h),(l||!Yt(d))&&(r(c),o&16))for(let g=0;g<a.length;g++){const x=a[g];n(x,e,i,!0,!!x.dynamicChildren)}},move:P1,hydrate:Eo};function P1(t,e,i,{o:{insert:s},m:n},r=2){r===0&&s(t.targetAnchor,e,i);const{el:l,anchor:o,shapeFlag:a,children:c,props:h}=t,p=r===2;if(p&&s(l,e,i),(!p||Yt(h))&&a&16)for(let d=0;d<c.length;d++)n(c[d],e,i,2);p&&s(o,e,i)}function Eo(t,e,i,s,n,r,{o:{nextSibling:l,parentNode:o,querySelector:a}},c){const h=e.target=fs(e.props,a);if(h){const p=h._lpa||h.firstChild;if(e.shapeFlag&16)if(Yt(e.props))e.anchor=c(l(t),e,o(t),i,s,n,r),e.targetAnchor=p;else{e.anchor=l(t);let d=p;for(;d;)if(d=l(d),d&&d.nodeType===8&&d.data==="teleport anchor"){e.targetAnchor=d,h._lpa=e.targetAnchor&&l(e.targetAnchor);break}c(p,e,h,i,s,n,r)}}return e.anchor&&l(e.anchor)}const Jc=$o,r0=Symbol(void 0),Xs=Symbol(void 0),Je=Symbol(void 0),B1=Symbol(void 0),Zt=[];let We=null;function Xr(t=!1){Zt.push(We=t?null:[])}function So(){Zt.pop(),We=Zt[Zt.length-1]||null}let n1=1;function Rn(t){n1+=t}function Wr(t){return t.dynamicChildren=n1>0?We||ut:null,So(),n1>0&&We&&We.push(t),t}function Qc(t,e,i,s,n,r){return Wr(Jr(t,e,i,s,n,r,!0))}function Yr(t,e,i,s,n){return Wr(Me(t,e,i,s,n,!0))}function X1(t){return t?t.__v_isVNode===!0:!1}function G0(t,e){return t.type===e.type&&t.key===e.key}const ui="__vInternal",Zr=({key:t})=>t!=null?t:null,N1=({ref:t,ref_key:e,ref_for:i})=>t!=null?we(t)||Se(t)||Y(t)?{i:Ve,r:t,k:e,f:!!i}:t:null;function Jr(t,e=null,i=null,s=0,n=null,r=t===r0?0:1,l=!1,o=!1){const a={__v_isVNode:!0,__v_skip:!0,type:t,props:e,key:e&&Zr(e),ref:e&&N1(e),scopeId:Pr,slotScopeIds:null,children:i,component:null,suspense:null,ssContent:null,ssFallback:null,dirs:null,transition:null,el:null,anchor:null,target:null,targetAnchor:null,staticCount:0,shapeFlag:r,patchFlag:s,dynamicProps:n,dynamicChildren:null,appContext:null};return o?(Ws(a,i),r&128&&t.normalize(a)):i&&(a.shapeFlag|=we(i)?8:16),n1>0&&!l&&We&&(a.patchFlag>0||r&6)&&a.patchFlag!==32&&We.push(a),a}const Me=Po;function Po(t,e=null,i=null,s=0,n=null,r=!1){if((!t||t===lo)&&(t=Je),X1(t)){const o=u0(t,e,!0);return i&&Ws(o,i),n1>0&&!r&&We&&(o.shapeFlag&6?We[We.indexOf(t)]=o:We.push(o)),o.patchFlag|=-2,o}if(No(t)&&(t=t.__vccOpts),e){e=Do(e);let{class:o,style:a}=e;o&&!we(o)&&(e.class=Ps(o)),be(a)&&(gr(a)&&!q(a)&&(a=Pe({},a)),e.style=Ss(a))}const l=we(t)?1:Dr(t)?128:Co(t)?64:be(t)?4:Y(t)?2:0;return Jr(t,e,i,s,n,l,r,!0)}function Do(t){return t?gr(t)||ui in t?Pe({},t):t:null}function u0(t,e,i=!1){const{props:s,ref:n,patchFlag:r,children:l}=t,o=e?Oo(s||{},e):s;return{__v_isVNode:!0,__v_skip:!0,type:t.type,props:o,key:o&&Zr(o),ref:e&&e.ref?i&&n?q(n)?n.concat(N1(e)):[n,N1(e)]:N1(e):n,scopeId:t.scopeId,slotScopeIds:t.slotScopeIds,children:l,target:t.target,targetAnchor:t.targetAnchor,staticCount:t.staticCount,shapeFlag:t.shapeFlag,patchFlag:e&&t.type!==r0?r===-1?16:r|16:r,dynamicProps:t.dynamicProps,dynamicChildren:t.dynamicChildren,appContext:t.appContext,dirs:t.dirs,transition:t.transition,component:t.component,suspense:t.suspense,ssContent:t.ssContent&&u0(t.ssContent),ssFallback:t.ssFallback&&u0(t.ssFallback),el:t.el,anchor:t.anchor}}function Ao(t=" ",e=0){return Me(Xs,null,t,e)}function e5(t,e){const i=Me(B1,null,t);return i.staticCount=e,i}function t5(t="",e=!1){return e?(Xr(),Yr(Je,null,t)):Me(Je,null,t)}function l0(t){return t==null||typeof t=="boolean"?Me(Je):q(t)?Me(r0,null,t.slice()):typeof t=="object"?z0(t):Me(Xs,null,String(t))}function z0(t){return t.el===null||t.memo?t:u0(t)}function Ws(t,e){let i=0;const{shapeFlag:s}=t;if(e==null)e=null;else if(q(e))i=16;else if(typeof e=="object")if(s&65){const n=e.default;n&&(n._c&&(n._d=!1),Ws(t,n()),n._c&&(n._d=!0));return}else{i=32;const n=e._;!n&&!(ui in e)?e._ctx=Ve:n===3&&Ve&&(Ve.slots._===1?e._=1:(e._=2,t.patchFlag|=1024))}else Y(e)?(e={default:e,_ctx:Ve},i=32):(e=String(e),s&64?(i=16,e=[Ao(e)]):i=8);t.children=e,t.shapeFlag|=i}function Oo(...t){const e={};for(let i=0;i<t.length;i++){const s=t[i];for(const n in s)if(n==="class")e.class!==s.class&&(e.class=Ps([e.class,s.class]));else if(n==="style")e.style=Ss([e.style,s.style]);else if(si(n)){const r=e[n],l=s[n];l&&r!==l&&!(q(r)&&r.includes(l))&&(e[n]=r?[].concat(r,l):l)}else n!==""&&(e[n]=s[n])}return e}function Ue(t,e,i,s=null){Ge(t,e,7,[i,s])}const Ro=Kr();let To=0;function Mo(t,e,i){const s=t.type,n=(e?e.appContext:t.appContext)||Ro,r={uid:To++,vnode:t,type:s,parent:e,appContext:n,root:null,next:null,subTree:null,effect:null,update:null,scope:new nr(!0),render:null,proxy:null,exposed:null,exposeProxy:null,withProxy:null,provides:e?e.provides:Object.create(n.provides),accessCache:null,renderCache:[],components:null,directives:null,propsOptions:Ur(s,n),emitsOptions:Sr(s,n),emit:null,emitted:null,propsDefaults:de,inheritAttrs:s.inheritAttrs,ctx:de,data:de,props:de,attrs:de,slots:de,refs:de,setupState:de,setupContext:null,suspense:i,suspenseId:i?i.pendingId:0,asyncDep:null,asyncResolved:!1,isMounted:!1,isUnmounted:!1,isDeactivated:!1,bc:null,c:null,bm:null,m:null,bu:null,u:null,um:null,bum:null,da:null,a:null,rtg:null,rtc:null,ec:null,sp:null};return r.ctx={_:r},r.root=e?e.root:r,r.emit=B4.bind(null,r),t.ce&&t.ce(r),r}let Ce=null;const Qr=()=>Ce||Ve,xt=t=>{Ce=t,t.scope.on()},W0=()=>{Ce&&Ce.scope.off(),Ce=null};function el(t){return t.vnode.shapeFlag&4}let r1=!1;function Fo(t,e=!1){r1=e;const{props:i,children:s}=t.vnode,n=el(t);mo(t,i,n,e),xo(t,s);const r=n?Io(t,e):void 0;return r1=!1,r}function Io(t,e){const i=t.type;t.accessCache=Object.create(null),t.proxy=ai(new Proxy(t.ctx,ao));const{setup:s}=i;if(s){const n=t.setupContext=s.length>1?Bo(t):null;xt(t),Dt();const r=S0(s,t,0,[t.props,n]);if(At(),W0(),er(r)){if(r.then(W0,W0),e)return r.then(l=>{Tn(t,l,e)}).catch(l=>{ci(l,t,0)});t.asyncDep=r}else Tn(t,r,e)}else tl(t,e)}function Tn(t,e,i){Y(e)?t.type.__ssrInlineRender?t.ssrRender=e:t.render=e:be(e)&&(t.setupState=wr(e)),tl(t,i)}let Mn;function tl(t,e,i){const s=t.type;if(!t.render){if(!e&&Mn&&!s.render){const n=s.template;if(n){const{isCustomElement:r,compilerOptions:l}=t.appContext.config,{delimiters:o,compilerOptions:a}=s,c=Pe(Pe({isCustomElement:r,delimiters:o},l),a);s.render=Mn(n,c)}}t.render=s.render||Ze}xt(t),Dt(),co(t),At(),W0()}function jo(t){return new Proxy(t.attrs,{get(e,i){return Ne(t,"get","$attrs"),e[i]}})}function Bo(t){const e=s=>{t.exposed=s||{}};let i;return{get attrs(){return i||(i=jo(t))},slots:t.slots,emit:t.emit,expose:e}}function mi(t){if(t.exposed)return t.exposeProxy||(t.exposeProxy=new Proxy(wr(ai(t.exposed)),{get(e,i){if(i in e)return e[i];if(i in q1)return q1[i](t)}}))}function ds(t){return Y(t)&&t.displayName||t.name}function No(t){return Y(t)&&"__vccOpts"in t}const o0=(t,e)=>R4(t,e,r1);function Ys(t,e,i){const s=arguments.length;return s===2?be(e)&&!q(e)?X1(e)?Me(t,null,[e]):Me(t,e):Me(t,null,e):(s>3?i=Array.prototype.slice.call(arguments,2):s===3&&X1(i)&&(i=[i]),Me(t,e,i))}const Lo="3.2.36",Ho="http://www.w3.org/2000/svg",q0=typeof document!="undefined"?document:null,Fn=q0&&q0.createElement("template"),Uo={insert:(t,e,i)=>{e.insertBefore(t,i||null)},remove:t=>{const e=t.parentNode;e&&e.removeChild(t)},createElement:(t,e,i,s)=>{const n=e?q0.createElementNS(Ho,t):q0.createElement(t,i?{is:i}:void 0);return t==="select"&&s&&s.multiple!=null&&n.setAttribute("multiple",s.multiple),n},createText:t=>q0.createTextNode(t),createComment:t=>q0.createComment(t),setText:(t,e)=>{t.nodeValue=e},setElementText:(t,e)=>{t.textContent=e},parentNode:t=>t.parentNode,nextSibling:t=>t.nextSibling,querySelector:t=>q0.querySelector(t),setScopeId(t,e){t.setAttribute(e,"")},cloneNode(t){const e=t.cloneNode(!0);return"_value"in t&&(e._value=t._value),e},insertStaticContent(t,e,i,s,n,r){const l=i?i.previousSibling:e.lastChild;if(n&&(n===r||n.nextSibling))for(;e.insertBefore(n.cloneNode(!0),i),!(n===r||!(n=n.nextSibling)););else{Fn.innerHTML=s?`<svg>${t}</svg>`:t;const o=Fn.content;if(s){const a=o.firstChild;for(;a.firstChild;)o.appendChild(a.firstChild);o.removeChild(a)}e.insertBefore(o,i)}return[l?l.nextSibling:e.firstChild,i?i.previousSibling:e.lastChild]}};function Vo(t,e,i){const s=t._vtc;s&&(e=(e?[e,...s]:[...s]).join(" ")),e==null?t.removeAttribute("class"):i?t.setAttribute("class",e):t.className=e}function Go(t,e,i){const s=t.style,n=we(i);if(i&&!n){for(const r in i)ps(s,r,i[r]);if(e&&!we(e))for(const r in e)i[r]==null&&ps(s,r,"")}else{const r=s.display;n?e!==i&&(s.cssText=i):e&&t.removeAttribute("style"),"_vod"in t&&(s.display=r)}}const In=/\s*!important$/;function ps(t,e,i){if(q(i))i.forEach(s=>ps(t,e,s));else if(i==null&&(i=""),e.startsWith("--"))t.setProperty(e,i);else{const s=qo(t,e);In.test(i)?t.setProperty(Pt(s),i.replace(In,""),"important"):t[s]=i}}const jn=["Webkit","Moz","ms"],Ti={};function qo(t,e){const i=Ti[e];if(i)return i;let s=a0(e);if(s!=="filter"&&s in t)return Ti[e]=s;s=li(s);for(let n=0;n<jn.length;n++){const r=jn[n]+s;if(r in t)return Ti[e]=r}return e}const Bn="http://www.w3.org/1999/xlink";function Ko(t,e,i,s,n){if(s&&e.startsWith("xlink:"))i==null?t.removeAttributeNS(Bn,e.slice(6,e.length)):t.setAttributeNS(Bn,e,i);else{const r=Vl(e);i==null||r&&!Z2(i)?t.removeAttribute(e):t.setAttribute(e,r?"":i)}}function Xo(t,e,i,s,n,r,l){if(e==="innerHTML"||e==="textContent"){s&&l(s,n,r),t[e]=i==null?"":i;return}if(e==="value"&&t.tagName!=="PROGRESS"&&!t.tagName.includes("-")){t._value=i;const a=i==null?"":i;(t.value!==a||t.tagName==="OPTION")&&(t.value=a),i==null&&t.removeAttribute(e);return}let o=!1;if(i===""||i==null){const a=typeof t[e];a==="boolean"?i=Z2(i):i==null&&a==="string"?(i="",o=!0):a==="number"&&(i=0,o=!0)}try{t[e]=i}catch{}o&&t.removeAttribute(e)}const[il,Wo]=(()=>{let t=Date.now,e=!1;if(typeof window!="undefined"){Date.now()>document.createEvent("Event").timeStamp&&(t=performance.now.bind(performance));const i=navigator.userAgent.match(/firefox\/(\d+)/i);e=!!(i&&Number(i[1])<=53)}return[t,e]})();let us=0;const Yo=Promise.resolve(),Zo=()=>{us=0},Jo=()=>us||(Yo.then(Zo),us=il());function Qo(t,e,i,s){t.addEventListener(e,i,s)}function ea(t,e,i,s){t.removeEventListener(e,i,s)}function ta(t,e,i,s,n=null){const r=t._vei||(t._vei={}),l=r[e];if(s&&l)l.value=s;else{const[o,a]=ia(e);if(s){const c=r[e]=sa(s,n);Qo(t,o,c,a)}else l&&(ea(t,o,l,a),r[e]=void 0)}}const Nn=/(?:Once|Passive|Capture)$/;function ia(t){let e;if(Nn.test(t)){e={};let i;for(;i=t.match(Nn);)t=t.slice(0,t.length-i[0].length),e[i[0].toLowerCase()]=!0}return[Pt(t.slice(2)),e]}function sa(t,e){const i=s=>{const n=s.timeStamp||il();(Wo||n>=i.attached-1)&&Ge(na(s,i.value),e,5,[s])};return i.value=t,i.attached=Jo(),i}function na(t,e){if(q(e)){const i=t.stopImmediatePropagation;return t.stopImmediatePropagation=()=>{i.call(t),t._stopped=!0},e.map(s=>n=>!n._stopped&&s&&s(n))}else return e}const Ln=/^on[a-z]/,ra=(t,e,i,s,n=!1,r,l,o,a)=>{e==="class"?Vo(t,s,n):e==="style"?Go(t,i,s):si(e)?Ds(e)||ta(t,e,i,s,l):(e[0]==="."?(e=e.slice(1),!0):e[0]==="^"?(e=e.slice(1),!1):la(t,e,s,n))?Xo(t,e,s,r,l,o,a):(e==="true-value"?t._trueValue=s:e==="false-value"&&(t._falseValue=s),Ko(t,e,s,n))};function la(t,e,i,s){return s?!!(e==="innerHTML"||e==="textContent"||e in t&&Ln.test(e)&&Y(i)):e==="spellcheck"||e==="draggable"||e==="translate"||e==="form"||e==="list"&&t.tagName==="INPUT"||e==="type"&&t.tagName==="TEXTAREA"||Ln.test(e)&&we(i)?!1:e in t}const x0="transition",It="animation",sl=(t,{slots:e})=>Ys(Rr,oa(t),e);sl.displayName="Transition";const nl={name:String,type:String,css:{type:Boolean,default:!0},duration:[String,Number,Object],enterFromClass:String,enterActiveClass:String,enterToClass:String,appearFromClass:String,appearActiveClass:String,appearToClass:String,leaveFromClass:String,leaveActiveClass:String,leaveToClass:String};sl.props=Pe({},Rr.props,nl);const L0=(t,e=[])=>{q(t)?t.forEach(i=>i(...e)):t&&t(...e)},Hn=t=>t?q(t)?t.some(e=>e.length>1):t.length>1:!1;function oa(t){const e={};for(const T in t)T in nl||(e[T]=t[T]);if(t.css===!1)return e;const{name:i="v",type:s,duration:n,enterFromClass:r=`${i}-enter-from`,enterActiveClass:l=`${i}-enter-active`,enterToClass:o=`${i}-enter-to`,appearFromClass:a=r,appearActiveClass:c=l,appearToClass:h=o,leaveFromClass:p=`${i}-leave-from`,leaveActiveClass:d=`${i}-leave-active`,leaveToClass:g=`${i}-leave-to`}=t,x=aa(n),A=x&&x[0],C=x&&x[1],{onBeforeEnter:D,onEnter:z,onEnterCancelled:O,onLeave:P,onLeaveCancelled:W,onBeforeAppear:J=D,onAppear:K=z,onAppearCancelled:N=O}=e,Q=(T,me,Ae)=>{H0(T,me?h:o),H0(T,me?c:l),Ae&&Ae()},ie=(T,me)=>{T._isLeaving=!1,H0(T,p),H0(T,g),H0(T,d),me&&me()},le=T=>(me,Ae)=>{const h0=T?K:z,ye=()=>Q(me,T,Ae);L0(h0,[me,ye]),Un(()=>{H0(me,T?a:r),v0(me,T?h:o),Hn(h0)||Vn(me,s,A,ye)})};return Pe(e,{onBeforeEnter(T){L0(D,[T]),v0(T,r),v0(T,l)},onBeforeAppear(T){L0(J,[T]),v0(T,a),v0(T,c)},onEnter:le(!1),onAppear:le(!0),onLeave(T,me){T._isLeaving=!0;const Ae=()=>ie(T,me);v0(T,p),fa(),v0(T,d),Un(()=>{!T._isLeaving||(H0(T,p),v0(T,g),Hn(P)||Vn(T,s,C,Ae))}),L0(P,[T,Ae])},onEnterCancelled(T){Q(T,!1),L0(O,[T])},onAppearCancelled(T){Q(T,!0),L0(N,[T])},onLeaveCancelled(T){ie(T),L0(W,[T])}})}function aa(t){if(t==null)return null;if(be(t))return[Mi(t.enter),Mi(t.leave)];{const e=Mi(t);return[e,e]}}function Mi(t){return sr(t)}function v0(t,e){e.split(/\s+/).forEach(i=>i&&t.classList.add(i)),(t._vtc||(t._vtc=new Set)).add(e)}function H0(t,e){e.split(/\s+/).forEach(s=>s&&t.classList.remove(s));const{_vtc:i}=t;i&&(i.delete(e),i.size||(t._vtc=void 0))}function Un(t){requestAnimationFrame(()=>{requestAnimationFrame(t)})}let ca=0;function Vn(t,e,i,s){const n=t._endId=++ca,r=()=>{n===t._endId&&s()};if(i)return setTimeout(r,i);const{type:l,timeout:o,propCount:a}=ha(t,e);if(!l)return s();const c=l+"end";let h=0;const p=()=>{t.removeEventListener(c,d),r()},d=g=>{g.target===t&&++h>=a&&p()};setTimeout(()=>{h<a&&p()},o+1),t.addEventListener(c,d)}function ha(t,e){const i=window.getComputedStyle(t),s=x=>(i[x]||"").split(", "),n=s(x0+"Delay"),r=s(x0+"Duration"),l=Gn(n,r),o=s(It+"Delay"),a=s(It+"Duration"),c=Gn(o,a);let h=null,p=0,d=0;e===x0?l>0&&(h=x0,p=l,d=r.length):e===It?c>0&&(h=It,p=c,d=a.length):(p=Math.max(l,c),h=p>0?l>c?x0:It:null,d=h?h===x0?r.length:a.length:0);const g=h===x0&&/\b(transform|all)(,|$)/.test(i[x0+"Property"]);return{type:h,timeout:p,propCount:d,hasTransform:g}}function Gn(t,e){for(;t.length<e.length;)t=t.concat(t);return Math.max(...e.map((i,s)=>qn(i)+qn(t[s])))}function qn(t){return Number(t.slice(0,-1).replace(",","."))*1e3}function fa(){return document.body.offsetHeight}const da=["ctrl","shift","alt","meta"],pa={stop:t=>t.stopPropagation(),prevent:t=>t.preventDefault(),self:t=>t.target!==t.currentTarget,ctrl:t=>!t.ctrlKey,shift:t=>!t.shiftKey,alt:t=>!t.altKey,meta:t=>!t.metaKey,left:t=>"button"in t&&t.button!==0,middle:t=>"button"in t&&t.button!==1,right:t=>"button"in t&&t.button!==2,exact:(t,e)=>da.some(i=>t[`${i}Key`]&&!e.includes(i))},i5=(t,e)=>(i,...s)=>{for(let n=0;n<e.length;n++){const r=pa[e[n]];if(r&&r(i,e))return}return t(i,...s)},ua=Pe({patchProp:ra},Uo);let Kn;function ma(){return Kn||(Kn=ko(ua))}const ga=(...t)=>{const e=ma().createApp(...t),{mount:i}=e;return e.mount=s=>{const n=ya(s);if(!n)return;const r=e._component;!Y(r)&&!r.render&&!r.template&&(r.template=n.innerHTML),n.innerHTML="";const l=i(n,!1,n instanceof SVGElement);return n instanceof Element&&(n.removeAttribute("v-cloak"),n.setAttribute("data-v-app","")),l},e};function ya(t){return we(t)?document.querySelector(t):t}function Zs(t,e,i,s){Object.defineProperty(t,e,{get:i,set:s,enumerable:!0})}const Y0=Ls(!1);let gi;function xa(t,e){const i=/(edg|edge|edga|edgios)\/([\w.]+)/.exec(t)||/(opr)[\/]([\w.]+)/.exec(t)||/(vivaldi)[\/]([\w.]+)/.exec(t)||/(chrome|crios)[\/]([\w.]+)/.exec(t)||/(version)(applewebkit)[\/]([\w.]+).*(safari)[\/]([\w.]+)/.exec(t)||/(webkit)[\/]([\w.]+).*(version)[\/]([\w.]+).*(safari)[\/]([\w.]+)/.exec(t)||/(firefox|fxios)[\/]([\w.]+)/.exec(t)||/(webkit)[\/]([\w.]+)/.exec(t)||/(opera)(?:.*version|)[\/]([\w.]+)/.exec(t)||[];return{browser:i[5]||i[3]||i[1]||"",version:i[2]||i[4]||"0",versionNumber:i[4]||i[2]||"0",platform:e[0]||""}}function va(t){return/(ipad)/.exec(t)||/(ipod)/.exec(t)||/(windows phone)/.exec(t)||/(iphone)/.exec(t)||/(kindle)/.exec(t)||/(silk)/.exec(t)||/(android)/.exec(t)||/(win)/.exec(t)||/(mac)/.exec(t)||/(linux)/.exec(t)||/(cros)/.exec(t)||/(playbook)/.exec(t)||/(bb)/.exec(t)||/(blackberry)/.exec(t)||[]}const rl="ontouchstart"in window||window.navigator.maxTouchPoints>0;function wa(t){gi={is:j0({},t)},delete t.mac,delete t.desktop;const e=Math.min(window.innerHeight,window.innerWidth)>414?"ipad":"iphone";Object.assign(t,{mobile:!0,ios:!0,platform:e,[e]:!0})}function ba(t){const e=t.toLowerCase(),i=va(e),s=xa(e,i),n={};s.browser&&(n[s.browser]=!0,n.version=s.version,n.versionNumber=parseInt(s.versionNumber,10)),s.platform&&(n[s.platform]=!0);const r=n.android||n.ios||n.bb||n.blackberry||n.ipad||n.iphone||n.ipod||n.kindle||n.playbook||n.silk||n["windows phone"];return r===!0||e.indexOf("mobile")>-1?(n.mobile=!0,n.edga||n.edgios?(n.edge=!0,s.browser="edge"):n.crios?(n.chrome=!0,s.browser="chrome"):n.fxios&&(n.firefox=!0,s.browser="firefox")):n.desktop=!0,(n.ipod||n.ipad||n.iphone)&&(n.ios=!0),n["windows phone"]&&(n.winphone=!0,delete n["windows phone"]),(n.chrome||n.opr||n.safari||n.vivaldi||n.mobile===!0&&n.ios!==!0&&r!==!0)&&(n.webkit=!0),n.edg&&(s.browser="edgechromium",n.edgeChromium=!0),(n.safari&&n.blackberry||n.bb)&&(s.browser="blackberry",n.blackberry=!0),n.safari&&n.playbook&&(s.browser="playbook",n.playbook=!0),n.opr&&(s.browser="opera",n.opera=!0),n.safari&&n.android&&(s.browser="android",n.android=!0),n.safari&&n.kindle&&(s.browser="kindle",n.kindle=!0),n.safari&&n.silk&&(s.browser="silk",n.silk=!0),n.vivaldi&&(s.browser="vivaldi",n.vivaldi=!0),n.name=s.browser,n.platform=s.platform,e.indexOf("electron")>-1?n.electron=!0:document.location.href.indexOf("-extension://")>-1?n.bex=!0:(window.Capacitor!==void 0?(n.capacitor=!0,n.nativeMobile=!0,n.nativeMobileWrapper="capacitor"):(window._cordovaNative!==void 0||window.cordova!==void 0)&&(n.cordova=!0,n.nativeMobile=!0,n.nativeMobileWrapper="cordova"),rl===!0&&n.mac===!0&&(n.desktop===!0&&n.safari===!0||n.nativeMobile===!0&&n.android!==!0&&n.ios!==!0&&n.ipad!==!0)&&wa(n)),n}const Xn=navigator.userAgent||navigator.vendor||window.opera,ka={has:{touch:!1,webStorage:!1},within:{iframe:!1}},Ye={userAgent:Xn,is:ba(Xn),has:{touch:rl},within:{iframe:window.self!==window.top}},ms={install(t){const{$q:e}=t;Y0.value===!0?(t.onSSRHydrated.push(()=>{Y0.value=!1,Object.assign(e.platform,Ye),gi=void 0}),e.platform=Q0(this)):e.platform=this}};{let t;Zs(Ye.has,"webStorage",()=>{if(t!==void 0)return t;try{if(window.localStorage)return t=!0,!0}catch{}return t=!1,!1}),Ye.is.ios===!0&&window.navigator.vendor.toLowerCase().indexOf("apple"),Y0.value===!0?Object.assign(ms,Ye,gi,ka):Object.assign(ms,Ye)}var yi=(t,e)=>{const i=Q0(t);for(const s in t)Zs(e,s,()=>i[s],n=>{i[s]=n});return e};const vt={hasPassive:!1,passiveCapture:!0,notPassiveCapture:!0};try{const t=Object.defineProperty({},"passive",{get(){Object.assign(vt,{hasPassive:!0,passive:{passive:!0},notPassive:{passive:!1},passiveCapture:{passive:!0,capture:!0},notPassiveCapture:{passive:!1,capture:!0}})}});window.addEventListener("qtest",null,t),window.removeEventListener("qtest",null,t)}catch{}function l1(){}function s5(t){return t.button===0}function n5(t){return t.touches&&t.touches[0]?t=t.touches[0]:t.changedTouches&&t.changedTouches[0]?t=t.changedTouches[0]:t.targetTouches&&t.targetTouches[0]&&(t=t.targetTouches[0]),{top:t.clientY,left:t.clientX}}function r5(t){if(t.path)return t.path;if(t.composedPath)return t.composedPath();const e=[];let i=t.target;for(;i;){if(e.push(i),i.tagName==="HTML")return e.push(document),e.push(window),e;i=i.parentElement}}function l5(t){t.stopPropagation()}function Wn(t){t.cancelable!==!1&&t.preventDefault()}function o5(t){t.cancelable!==!1&&t.preventDefault(),t.stopPropagation()}function a5(t,e){if(t===void 0||e===!0&&t.__dragPrevented===!0)return;const i=e===!0?s=>{s.__dragPrevented=!0,s.addEventListener("dragstart",Wn,vt.notPassiveCapture)}:s=>{delete s.__dragPrevented,s.removeEventListener("dragstart",Wn,vt.notPassiveCapture)};t.querySelectorAll("a, img").forEach(i)}function c5(t,e,i){const s=`__q_${e}_evt`;t[s]=t[s]!==void 0?t[s].concat(i):i,i.forEach(n=>{n[0].addEventListener(n[1],t[n[2]],vt[n[3]])})}function h5(t,e){const i=`__q_${e}_evt`;t[i]!==void 0&&(t[i].forEach(s=>{s[0].removeEventListener(s[1],t[s[2]],vt[s[3]])}),t[i]=void 0)}function za(t,e=250,i){let s;function n(){const r=arguments,l=()=>{s=void 0,i!==!0&&t.apply(this,r)};clearTimeout(s),i===!0&&s===void 0&&t.apply(this,r),s=setTimeout(l,e)}return n.cancel=()=>{clearTimeout(s)},n}const Fi=["sm","md","lg","xl"],{passive:Yn}=vt;var _a=yi({width:0,height:0,name:"xs",sizes:{sm:600,md:1024,lg:1440,xl:1920},lt:{sm:!0,md:!0,lg:!0,xl:!0},gt:{xs:!1,sm:!1,md:!1,lg:!1},xs:!0,sm:!1,md:!1,lg:!1,xl:!1},{setSizes:l1,setDebounce:l1,install({$q:t,onSSRHydrated:e}){if(t.screen=this,this.__installed===!0){t.config.screen!==void 0&&(t.config.screen.bodyClasses===!1?document.body.classList.remove(`screen--${this.name}`):this.__update(!0));return}const{visualViewport:i}=window,s=i||window,n=document.scrollingElement||document.documentElement,r=i===void 0||Ye.is.mobile===!0?()=>[Math.max(window.innerWidth,n.clientWidth),Math.max(window.innerHeight,n.clientHeight)]:()=>[i.width*i.scale+window.innerWidth-n.clientWidth,i.height*i.scale+window.innerHeight-n.clientHeight],l=t.config.screen!==void 0&&t.config.screen.bodyClasses===!0;this.__update=p=>{const[d,g]=r();if(g!==this.height&&(this.height=g),d!==this.width)this.width=d;else if(p!==!0)return;let x=this.sizes;this.gt.xs=d>=x.sm,this.gt.sm=d>=x.md,this.gt.md=d>=x.lg,this.gt.lg=d>=x.xl,this.lt.sm=d<x.sm,this.lt.md=d<x.md,this.lt.lg=d<x.lg,this.lt.xl=d<x.xl,this.xs=this.lt.sm,this.sm=this.gt.xs===!0&&this.lt.md===!0,this.md=this.gt.sm===!0&&this.lt.lg===!0,this.lg=this.gt.md===!0&&this.lt.xl===!0,this.xl=this.gt.lg,x=this.xs===!0&&"xs"||this.sm===!0&&"sm"||this.md===!0&&"md"||this.lg===!0&&"lg"||"xl",x!==this.name&&(l===!0&&(document.body.classList.remove(`screen--${this.name}`),document.body.classList.add(`screen--${x}`)),this.name=x)};let o,a={},c=16;this.setSizes=p=>{Fi.forEach(d=>{p[d]!==void 0&&(a[d]=p[d])})},this.setDebounce=p=>{c=p};const h=()=>{const p=getComputedStyle(document.body);p.getPropertyValue("--q-size-sm")&&Fi.forEach(d=>{this.sizes[d]=parseInt(p.getPropertyValue(`--q-size-${d}`),10)}),this.setSizes=d=>{Fi.forEach(g=>{d[g]&&(this.sizes[g]=d[g])}),this.__update(!0)},this.setDebounce=d=>{o!==void 0&&s.removeEventListener("resize",o,Yn),o=d>0?za(this.__update,d):this.__update,s.addEventListener("resize",o,Yn)},this.setDebounce(c),Object.keys(a).length>0?(this.setSizes(a),a=void 0):this.__update(),l===!0&&this.name==="xs"&&document.body.classList.add("screen--xs")};Y0.value===!0?e.push(h):h()}});const Oe=yi({isActive:!1,mode:!1},{__media:void 0,set(t){Oe.mode=t,t==="auto"?(Oe.__media===void 0&&(Oe.__media=window.matchMedia("(prefers-color-scheme: dark)"),Oe.__updateMedia=()=>{Oe.set("auto")},Oe.__media.addListener(Oe.__updateMedia)),t=Oe.__media.matches):Oe.__media!==void 0&&(Oe.__media.removeListener(Oe.__updateMedia),Oe.__media=void 0),Oe.isActive=t===!0,document.body.classList.remove(`body--${t===!0?"light":"dark"}`),document.body.classList.add(`body--${t===!0?"dark":"light"}`)},toggle(){Oe.set(Oe.isActive===!1)},install({$q:t,onSSRHydrated:e,ssrContext:i}){const{dark:s}=t.config;if(t.dark=this,this.__installed===!0&&s===void 0)return;this.isActive=s===!0;const n=s!==void 0?s:!1;if(Y0.value===!0){const r=o=>{this.__fromSSR=o},l=this.set;this.set=r,r(n),e.push(()=>{this.set=l,this.set(this.__fromSSR)})}else this.set(n)}}),ll=()=>!0;function Ca(t){return typeof t=="string"&&t!==""&&t!=="/"&&t!=="#/"}function $a(t){return t.startsWith("#")===!0&&(t=t.substring(1)),t.startsWith("/")===!1&&(t="/"+t),t.endsWith("/")===!0&&(t=t.substring(0,t.length-1)),"#"+t}function Ea(t){if(t.backButtonExit===!1)return()=>!1;if(t.backButtonExit==="*")return ll;const e=["#/"];return Array.isArray(t.backButtonExit)===!0&&e.push(...t.backButtonExit.filter(Ca).map($a)),()=>e.includes(window.location.hash)}var Sa={__history:[],add:l1,remove:l1,install({$q:t}){if(this.__installed===!0)return;const{cordova:e,capacitor:i}=Ye.is;if(e!==!0&&i!==!0)return;const s=t.config[e===!0?"cordova":"capacitor"];if(s!==void 0&&s.backButton===!1||i===!0&&(window.Capacitor===void 0||window.Capacitor.Plugins.App===void 0))return;this.add=l=>{l.condition===void 0&&(l.condition=ll),this.__history.push(l)},this.remove=l=>{const o=this.__history.indexOf(l);o>=0&&this.__history.splice(o,1)};const n=Ea(Object.assign({backButtonExit:!0},s)),r=()=>{if(this.__history.length){const l=this.__history[this.__history.length-1];l.condition()===!0&&(this.__history.pop(),l.handler())}else n()===!0?navigator.app.exitApp():window.history.back()};e===!0?document.addEventListener("deviceready",()=>{document.addEventListener("backbutton",r,!1)}):window.Capacitor.Plugins.App.addListener("backButton",r)}},Zn={isoName:"en-US",nativeName:"English (US)",label:{clear:"Clear",ok:"OK",cancel:"Cancel",close:"Close",set:"Set",select:"Select",reset:"Reset",remove:"Remove",update:"Update",create:"Create",search:"Search",filter:"Filter",refresh:"Refresh"},date:{days:"Sunday_Monday_Tuesday_Wednesday_Thursday_Friday_Saturday".split("_"),daysShort:"Sun_Mon_Tue_Wed_Thu_Fri_Sat".split("_"),months:"January_February_March_April_May_June_July_August_September_October_November_December".split("_"),monthsShort:"Jan_Feb_Mar_Apr_May_Jun_Jul_Aug_Sep_Oct_Nov_Dec".split("_"),firstDayOfWeek:0,format24h:!1,pluralDay:"days"},table:{noData:"No data available",noResults:"No matching records found",loading:"Loading...",selectedRecords:t=>t===1?"1 record selected.":(t===0?"No":t)+" records selected.",recordsPerPage:"Records per page:",allRows:"All",pagination:(t,e,i)=>t+"-"+e+" of "+i,columns:"Columns"},editor:{url:"URL",bold:"Bold",italic:"Italic",strikethrough:"Strikethrough",underline:"Underline",unorderedList:"Unordered List",orderedList:"Ordered List",subscript:"Subscript",superscript:"Superscript",hyperlink:"Hyperlink",toggleFullscreen:"Toggle Fullscreen",quote:"Quote",left:"Left align",center:"Center align",right:"Right align",justify:"Justify align",print:"Print",outdent:"Decrease indentation",indent:"Increase indentation",removeFormat:"Remove formatting",formatting:"Formatting",fontSize:"Font Size",align:"Align",hr:"Insert Horizontal Rule",undo:"Undo",redo:"Redo",heading1:"Heading 1",heading2:"Heading 2",heading3:"Heading 3",heading4:"Heading 4",heading5:"Heading 5",heading6:"Heading 6",paragraph:"Paragraph",code:"Code",size1:"Very small",size2:"A bit small",size3:"Normal",size4:"Medium-large",size5:"Big",size6:"Very big",size7:"Maximum",defaultFont:"Default Font",viewSource:"View Source"},tree:{noNodes:"No nodes available",noResults:"No matching nodes found"}};function Jn(){const t=Array.isArray(navigator.languages)===!0&&navigator.languages.length>0?navigator.languages[0]:navigator.language;if(typeof t=="string")return t.split(/[-_]/).map((e,i)=>i===0?e.toLowerCase():i>1||e.length<4?e.toUpperCase():e[0].toUpperCase()+e.slice(1).toLowerCase()).join("-")}const _0=yi({__langPack:{}},{getLocale:Jn,set(t=Zn,e){const i=Ft(j0({},t),{rtl:t.rtl===!0,getLocale:Jn});{const s=document.documentElement;s.setAttribute("dir",i.rtl===!0?"rtl":"ltr"),s.setAttribute("lang",i.isoName),i.set=_0.set,Object.assign(_0.__langPack,i),_0.props=i,_0.isoName=i.isoName,_0.nativeName=i.nativeName}},install({$q:t,lang:e,ssrContext:i}){t.lang=_0.__langPack,this.__installed===!0?e!==void 0&&this.set(e):this.set(e||Zn)}});function Pa(t,e,i=document.body){if(typeof t!="string")throw new TypeError("Expected a string as propName");if(typeof e!="string")throw new TypeError("Expected a string as value");if(!(i instanceof Element))throw new TypeError("Expected a DOM element");i.style.setProperty(`--q-${t}`,e)}let ol=!1;function Da(t){ol=t.isComposing===!0}function Aa(t){return ol===!0||t!==Object(t)||t.isComposing===!0||t.qKeyEvent===!0}function f5(t,e){return Aa(t)===!0?!1:[].concat(e).includes(t.keyCode)}function Oa(t){if(t.ios===!0)return"ios";if(t.android===!0)return"android"}function Ra({is:t,has:e,within:i},s){const n=[t.desktop===!0?"desktop":"mobile",`${e.touch===!1?"no-":""}touch`];if(t.mobile===!0){const r=Oa(t);r!==void 0&&n.push("platform-"+r)}if(t.nativeMobile===!0){const r=t.nativeMobileWrapper;n.push(r),n.push("native-mobile"),t.ios===!0&&(s[r]===void 0||s[r].iosStatusBarPadding!==!1)&&n.push("q-ios-padding")}else t.electron===!0?n.push("electron"):t.bex===!0&&n.push("bex");return i.iframe===!0&&n.push("within-iframe"),n}function Ta(){const t=document.body.className;let e=t;gi!==void 0&&(e=e.replace("desktop","platform-ios mobile")),Ye.has.touch===!0&&(e=e.replace("no-touch","touch")),Ye.within.iframe===!0&&(e+=" within-iframe"),t!==e&&(document.body.className=e)}function Ma(t){for(const e in t)Pa(e,t[e])}var Fa={install(t){if(this.__installed!==!0){if(Y0.value===!0)Ta();else{const{$q:e}=t;e.config.brand!==void 0&&Ma(e.config.brand);const i=Ra(Ye,e.config);document.body.classList.add.apply(document.body.classList,i)}Ye.is.ios===!0&&document.body.addEventListener("touchstart",l1),window.addEventListener("keydown",Da,!0)}}},Ia={name:"material-icons",type:{positive:"check_circle",negative:"warning",info:"info",warning:"priority_high"},arrow:{up:"arrow_upward",right:"arrow_forward",down:"arrow_downward",left:"arrow_back",dropdown:"arrow_drop_down"},chevron:{left:"chevron_left",right:"chevron_right"},colorPicker:{spectrum:"gradient",tune:"tune",palette:"style"},pullToRefresh:{icon:"refresh"},carousel:{left:"chevron_left",right:"chevron_right",up:"keyboard_arrow_up",down:"keyboard_arrow_down",navigationIcon:"lens"},chip:{remove:"cancel",selected:"check"},datetime:{arrowLeft:"chevron_left",arrowRight:"chevron_right",now:"access_time",today:"today"},editor:{bold:"format_bold",italic:"format_italic",strikethrough:"strikethrough_s",underline:"format_underlined",unorderedList:"format_list_bulleted",orderedList:"format_list_numbered",subscript:"vertical_align_bottom",superscript:"vertical_align_top",hyperlink:"link",toggleFullscreen:"fullscreen",quote:"format_quote",left:"format_align_left",center:"format_align_center",right:"format_align_right",justify:"format_align_justify",print:"print",outdent:"format_indent_decrease",indent:"format_indent_increase",removeFormat:"format_clear",formatting:"text_format",fontSize:"format_size",align:"format_align_left",hr:"remove",undo:"undo",redo:"redo",heading:"format_size",code:"code",size:"format_size",font:"font_download",viewSource:"code"},expansionItem:{icon:"keyboard_arrow_down",denseIcon:"arrow_drop_down"},fab:{icon:"add",activeIcon:"close"},field:{clear:"cancel",error:"error"},pagination:{first:"first_page",prev:"keyboard_arrow_left",next:"keyboard_arrow_right",last:"last_page"},rating:{icon:"grade"},stepper:{done:"check",active:"edit",error:"warning"},tabs:{left:"chevron_left",right:"chevron_right",up:"keyboard_arrow_up",down:"keyboard_arrow_down"},table:{arrowUp:"arrow_upward",warning:"warning",firstPage:"first_page",prevPage:"chevron_left",nextPage:"chevron_right",lastPage:"last_page"},tree:{icon:"play_arrow"},uploader:{done:"done",clear:"clear",add:"add_box",upload:"cloud_upload",removeQueue:"clear_all",removeUploaded:"done_all"}};const W1=yi({iconMapFn:null,__icons:{}},{set(t,e){const i=Ft(j0({},t),{rtl:t.rtl===!0});i.set=W1.set,Object.assign(W1.__icons,i)},install({$q:t,iconSet:e,ssrContext:i}){t.config.iconMapFn!==void 0&&(this.iconMapFn=t.config.iconMapFn),t.iconSet=this.__icons,Zs(t,"iconMapFn",()=>this.iconMapFn,s=>{this.iconMapFn=s}),this.__installed===!0?e!==void 0&&this.set(e):this.set(e||Ia)}}),ja="_q_",d5="_q_l_",p5="_q_pc_",u5="_q_fo_",m5="_q_tabs_",Qn={};let al=!1;function Ba(){al=!0}function e2(t){return t!==null&&typeof t=="object"&&Array.isArray(t)!==!0}function g5(t){return typeof t=="number"&&isFinite(t)}const t2=[ms,Fa,Oe,_a,Sa,_0,W1];function i2(t,e){e.forEach(i=>{i.install(t),i.__installed=!0})}function Na(t,e,i){t.config.globalProperties.$q=i.$q,t.provide(ja,i.$q),i2(i,t2),e.components!==void 0&&Object.values(e.components).forEach(s=>{e2(s)===!0&&s.name!==void 0&&t.component(s.name,s)}),e.directives!==void 0&&Object.values(e.directives).forEach(s=>{e2(s)===!0&&s.name!==void 0&&t.directive(s.name,s)}),e.plugins!==void 0&&i2(i,Object.values(e.plugins).filter(s=>typeof s.install=="function"&&t2.includes(s)===!1)),Y0.value===!0&&(i.$q.onSSRHydrated=()=>{i.onSSRHydrated.forEach(s=>{s()}),i.$q.onSSRHydrated=()=>{}})}var La=function(t,e={}){const i={version:"2.7.1"};al===!1?(e.config!==void 0&&Object.assign(Qn,e.config),i.config=j0({},Qn),Ba()):i.config=e.config||{},Na(t,e,{parentApp:t,$q:i,lang:e.lang,iconSet:e.iconSet,onSSRHydrated:[]})},Ha={version:"2.7.1",install:La,lang:_0,iconSet:W1};/**
 * @license
 * Copyright 2019 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const Js=window.ShadowRoot&&(window.ShadyCSS===void 0||window.ShadyCSS.nativeShadow)&&"adoptedStyleSheets"in Document.prototype&&"replace"in CSSStyleSheet.prototype,Qs=Symbol(),s2=new Map;class cl{constructor(e,i){if(this._$cssResult$=!0,i!==Qs)throw Error("CSSResult is not constructable. Use `unsafeCSS` or `css` instead.");this.cssText=e}get styleSheet(){let e=s2.get(this.cssText);return Js&&e===void 0&&(s2.set(this.cssText,e=new CSSStyleSheet),e.replaceSync(this.cssText)),e}toString(){return this.cssText}}const Ua=t=>new cl(typeof t=="string"?t:t+"",Qs),ke=(t,...e)=>{const i=t.length===1?t[0]:e.reduce((s,n,r)=>s+(l=>{if(l._$cssResult$===!0)return l.cssText;if(typeof l=="number")return l;throw Error("Value passed to 'css' function must be a 'css' function result: "+l+". Use 'unsafeCSS' to pass non-literal values, but take care to ensure page security.")})(n)+t[r+1],t[0]);return new cl(i,Qs)},Va=(t,e)=>{Js?t.adoptedStyleSheets=e.map(i=>i instanceof CSSStyleSheet?i:i.styleSheet):e.forEach(i=>{const s=document.createElement("style"),n=window.litNonce;n!==void 0&&s.setAttribute("nonce",n),s.textContent=i.cssText,t.appendChild(s)})},n2=Js?t=>t:t=>t instanceof CSSStyleSheet?(e=>{let i="";for(const s of e.cssRules)i+=s.cssText;return Ua(i)})(t):t;/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */var Ii;const r2=window.trustedTypes,Ga=r2?r2.emptyScript:"",l2=window.reactiveElementPolyfillSupport,gs={toAttribute(t,e){switch(e){case Boolean:t=t?Ga:null;break;case Object:case Array:t=t==null?t:JSON.stringify(t)}return t},fromAttribute(t,e){let i=t;switch(e){case Boolean:i=t!==null;break;case Number:i=t===null?null:Number(t);break;case Object:case Array:try{i=JSON.parse(t)}catch{i=null}}return i}},hl=(t,e)=>e!==t&&(e==e||t==t),ji={attribute:!0,type:String,converter:gs,reflect:!1,hasChanged:hl};class dt extends HTMLElement{constructor(){super(),this._$Et=new Map,this.isUpdatePending=!1,this.hasUpdated=!1,this._$Ei=null,this.o()}static addInitializer(e){var i;(i=this.l)!==null&&i!==void 0||(this.l=[]),this.l.push(e)}static get observedAttributes(){this.finalize();const e=[];return this.elementProperties.forEach((i,s)=>{const n=this._$Eh(s,i);n!==void 0&&(this._$Eu.set(n,s),e.push(n))}),e}static createProperty(e,i=ji){if(i.state&&(i.attribute=!1),this.finalize(),this.elementProperties.set(e,i),!i.noAccessor&&!this.prototype.hasOwnProperty(e)){const s=typeof e=="symbol"?Symbol():"__"+e,n=this.getPropertyDescriptor(e,s,i);n!==void 0&&Object.defineProperty(this.prototype,e,n)}}static getPropertyDescriptor(e,i,s){return{get(){return this[i]},set(n){const r=this[e];this[i]=n,this.requestUpdate(e,r,s)},configurable:!0,enumerable:!0}}static getPropertyOptions(e){return this.elementProperties.get(e)||ji}static finalize(){if(this.hasOwnProperty("finalized"))return!1;this.finalized=!0;const e=Object.getPrototypeOf(this);if(e.finalize(),this.elementProperties=new Map(e.elementProperties),this._$Eu=new Map,this.hasOwnProperty("properties")){const i=this.properties,s=[...Object.getOwnPropertyNames(i),...Object.getOwnPropertySymbols(i)];for(const n of s)this.createProperty(n,i[n])}return this.elementStyles=this.finalizeStyles(this.styles),!0}static finalizeStyles(e){const i=[];if(Array.isArray(e)){const s=new Set(e.flat(1/0).reverse());for(const n of s)i.unshift(n2(n))}else e!==void 0&&i.push(n2(e));return i}static _$Eh(e,i){const s=i.attribute;return s===!1?void 0:typeof s=="string"?s:typeof e=="string"?e.toLowerCase():void 0}o(){var e;this._$Ep=new Promise(i=>this.enableUpdating=i),this._$AL=new Map,this._$Em(),this.requestUpdate(),(e=this.constructor.l)===null||e===void 0||e.forEach(i=>i(this))}addController(e){var i,s;((i=this._$Eg)!==null&&i!==void 0?i:this._$Eg=[]).push(e),this.renderRoot!==void 0&&this.isConnected&&((s=e.hostConnected)===null||s===void 0||s.call(e))}removeController(e){var i;(i=this._$Eg)===null||i===void 0||i.splice(this._$Eg.indexOf(e)>>>0,1)}_$Em(){this.constructor.elementProperties.forEach((e,i)=>{this.hasOwnProperty(i)&&(this._$Et.set(i,this[i]),delete this[i])})}createRenderRoot(){var e;const i=(e=this.shadowRoot)!==null&&e!==void 0?e:this.attachShadow(this.constructor.shadowRootOptions);return Va(i,this.constructor.elementStyles),i}connectedCallback(){var e;this.renderRoot===void 0&&(this.renderRoot=this.createRenderRoot()),this.enableUpdating(!0),(e=this._$Eg)===null||e===void 0||e.forEach(i=>{var s;return(s=i.hostConnected)===null||s===void 0?void 0:s.call(i)})}enableUpdating(e){}disconnectedCallback(){var e;(e=this._$Eg)===null||e===void 0||e.forEach(i=>{var s;return(s=i.hostDisconnected)===null||s===void 0?void 0:s.call(i)})}attributeChangedCallback(e,i,s){this._$AK(e,s)}_$ES(e,i,s=ji){var n,r;const l=this.constructor._$Eh(e,s);if(l!==void 0&&s.reflect===!0){const o=((r=(n=s.converter)===null||n===void 0?void 0:n.toAttribute)!==null&&r!==void 0?r:gs.toAttribute)(i,s.type);this._$Ei=e,o==null?this.removeAttribute(l):this.setAttribute(l,o),this._$Ei=null}}_$AK(e,i){var s,n,r;const l=this.constructor,o=l._$Eu.get(e);if(o!==void 0&&this._$Ei!==o){const a=l.getPropertyOptions(o),c=a.converter,h=(r=(n=(s=c)===null||s===void 0?void 0:s.fromAttribute)!==null&&n!==void 0?n:typeof c=="function"?c:null)!==null&&r!==void 0?r:gs.fromAttribute;this._$Ei=o,this[o]=h(i,a.type),this._$Ei=null}}requestUpdate(e,i,s){let n=!0;e!==void 0&&(((s=s||this.constructor.getPropertyOptions(e)).hasChanged||hl)(this[e],i)?(this._$AL.has(e)||this._$AL.set(e,i),s.reflect===!0&&this._$Ei!==e&&(this._$EC===void 0&&(this._$EC=new Map),this._$EC.set(e,s))):n=!1),!this.isUpdatePending&&n&&(this._$Ep=this._$E_())}async _$E_(){this.isUpdatePending=!0;try{await this._$Ep}catch(i){Promise.reject(i)}const e=this.scheduleUpdate();return e!=null&&await e,!this.isUpdatePending}scheduleUpdate(){return this.performUpdate()}performUpdate(){var e;if(!this.isUpdatePending)return;this.hasUpdated,this._$Et&&(this._$Et.forEach((n,r)=>this[r]=n),this._$Et=void 0);let i=!1;const s=this._$AL;try{i=this.shouldUpdate(s),i?(this.willUpdate(s),(e=this._$Eg)===null||e===void 0||e.forEach(n=>{var r;return(r=n.hostUpdate)===null||r===void 0?void 0:r.call(n)}),this.update(s)):this._$EU()}catch(n){throw i=!1,this._$EU(),n}i&&this._$AE(s)}willUpdate(e){}_$AE(e){var i;(i=this._$Eg)===null||i===void 0||i.forEach(s=>{var n;return(n=s.hostUpdated)===null||n===void 0?void 0:n.call(s)}),this.hasUpdated||(this.hasUpdated=!0,this.firstUpdated(e)),this.updated(e)}_$EU(){this._$AL=new Map,this.isUpdatePending=!1}get updateComplete(){return this.getUpdateComplete()}getUpdateComplete(){return this._$Ep}shouldUpdate(e){return!0}update(e){this._$EC!==void 0&&(this._$EC.forEach((i,s)=>this._$ES(s,this[s],i)),this._$EC=void 0),this._$EU()}updated(e){}firstUpdated(e){}}dt.finalized=!0,dt.elementProperties=new Map,dt.elementStyles=[],dt.shadowRootOptions={mode:"open"},l2==null||l2({ReactiveElement:dt}),((Ii=globalThis.reactiveElementVersions)!==null&&Ii!==void 0?Ii:globalThis.reactiveElementVersions=[]).push("1.3.2");/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */var Bi;const wt=globalThis.trustedTypes,o2=wt?wt.createPolicy("lit-html",{createHTML:t=>t}):void 0,$0=`lit$${(Math.random()+"").slice(9)}$`,fl="?"+$0,qa=`<${fl}>`,bt=document,o1=(t="")=>bt.createComment(t),a1=t=>t===null||typeof t!="object"&&typeof t!="function",dl=Array.isArray,Ka=t=>{var e;return dl(t)||typeof((e=t)===null||e===void 0?void 0:e[Symbol.iterator])=="function"},jt=/<(?:(!--|\/[^a-zA-Z])|(\/?[a-zA-Z][^>\s]*)|(\/?$))/g,a2=/-->/g,c2=/>/g,U0=/>|[ 	\n\r](?:([^\s"'>=/]+)([ 	\n\r]*=[ 	\n\r]*(?:[^ 	\n\r"'`<>=]|("|')|))|$)/g,h2=/'/g,f2=/"/g,pl=/^(?:script|style|textarea|title)$/i,ul=t=>(e,...i)=>({_$litType$:t,strings:e,values:i}),G=ul(1),V=ul(2),A0=Symbol.for("lit-noChange"),_e=Symbol.for("lit-nothing"),d2=new WeakMap,Xa=(t,e,i)=>{var s,n;const r=(s=i==null?void 0:i.renderBefore)!==null&&s!==void 0?s:e;let l=r._$litPart$;if(l===void 0){const o=(n=i==null?void 0:i.renderBefore)!==null&&n!==void 0?n:null;r._$litPart$=l=new x1(e.insertBefore(o1(),o),o,void 0,i!=null?i:{})}return l._$AI(t),l},yt=bt.createTreeWalker(bt,129,null,!1),Wa=(t,e)=>{const i=t.length-1,s=[];let n,r=e===2?"<svg>":"",l=jt;for(let a=0;a<i;a++){const c=t[a];let h,p,d=-1,g=0;for(;g<c.length&&(l.lastIndex=g,p=l.exec(c),p!==null);)g=l.lastIndex,l===jt?p[1]==="!--"?l=a2:p[1]!==void 0?l=c2:p[2]!==void 0?(pl.test(p[2])&&(n=RegExp("</"+p[2],"g")),l=U0):p[3]!==void 0&&(l=U0):l===U0?p[0]===">"?(l=n!=null?n:jt,d=-1):p[1]===void 0?d=-2:(d=l.lastIndex-p[2].length,h=p[1],l=p[3]===void 0?U0:p[3]==='"'?f2:h2):l===f2||l===h2?l=U0:l===a2||l===c2?l=jt:(l=U0,n=void 0);const x=l===U0&&t[a+1].startsWith("/>")?" ":"";r+=l===jt?c+qa:d>=0?(s.push(h),c.slice(0,d)+"$lit$"+c.slice(d)+$0+x):c+$0+(d===-2?(s.push(void 0),a):x)}const o=r+(t[i]||"<?>")+(e===2?"</svg>":"");if(!Array.isArray(t)||!t.hasOwnProperty("raw"))throw Error("invalid template strings array");return[o2!==void 0?o2.createHTML(o):o,s]};class c1{constructor({strings:e,_$litType$:i},s){let n;this.parts=[];let r=0,l=0;const o=e.length-1,a=this.parts,[c,h]=Wa(e,i);if(this.el=c1.createElement(c,s),yt.currentNode=this.el.content,i===2){const p=this.el.content,d=p.firstChild;d.remove(),p.append(...d.childNodes)}for(;(n=yt.nextNode())!==null&&a.length<o;){if(n.nodeType===1){if(n.hasAttributes()){const p=[];for(const d of n.getAttributeNames())if(d.endsWith("$lit$")||d.startsWith($0)){const g=h[l++];if(p.push(d),g!==void 0){const x=n.getAttribute(g.toLowerCase()+"$lit$").split($0),A=/([.?@])?(.*)/.exec(g);a.push({type:1,index:r,name:A[2],strings:x,ctor:A[1]==="."?Za:A[1]==="?"?Qa:A[1]==="@"?e3:xi})}else a.push({type:6,index:r})}for(const d of p)n.removeAttribute(d)}if(pl.test(n.tagName)){const p=n.textContent.split($0),d=p.length-1;if(d>0){n.textContent=wt?wt.emptyScript:"";for(let g=0;g<d;g++)n.append(p[g],o1()),yt.nextNode(),a.push({type:2,index:++r});n.append(p[d],o1())}}}else if(n.nodeType===8)if(n.data===fl)a.push({type:2,index:r});else{let p=-1;for(;(p=n.data.indexOf($0,p+1))!==-1;)a.push({type:7,index:r}),p+=$0.length-1}r++}}static createElement(e,i){const s=bt.createElement("template");return s.innerHTML=e,s}}function kt(t,e,i=t,s){var n,r,l,o;if(e===A0)return e;let a=s!==void 0?(n=i._$Cl)===null||n===void 0?void 0:n[s]:i._$Cu;const c=a1(e)?void 0:e._$litDirective$;return(a==null?void 0:a.constructor)!==c&&((r=a==null?void 0:a._$AO)===null||r===void 0||r.call(a,!1),c===void 0?a=void 0:(a=new c(t),a._$AT(t,i,s)),s!==void 0?((l=(o=i)._$Cl)!==null&&l!==void 0?l:o._$Cl=[])[s]=a:i._$Cu=a),a!==void 0&&(e=kt(t,a._$AS(t,e.values),a,s)),e}class Ya{constructor(e,i){this.v=[],this._$AN=void 0,this._$AD=e,this._$AM=i}get parentNode(){return this._$AM.parentNode}get _$AU(){return this._$AM._$AU}p(e){var i;const{el:{content:s},parts:n}=this._$AD,r=((i=e==null?void 0:e.creationScope)!==null&&i!==void 0?i:bt).importNode(s,!0);yt.currentNode=r;let l=yt.nextNode(),o=0,a=0,c=n[0];for(;c!==void 0;){if(o===c.index){let h;c.type===2?h=new x1(l,l.nextSibling,this,e):c.type===1?h=new c.ctor(l,c.name,c.strings,this,e):c.type===6&&(h=new t3(l,this,e)),this.v.push(h),c=n[++a]}o!==(c==null?void 0:c.index)&&(l=yt.nextNode(),o++)}return r}m(e){let i=0;for(const s of this.v)s!==void 0&&(s.strings!==void 0?(s._$AI(e,s,i),i+=s.strings.length-2):s._$AI(e[i])),i++}}class x1{constructor(e,i,s,n){var r;this.type=2,this._$AH=_e,this._$AN=void 0,this._$AA=e,this._$AB=i,this._$AM=s,this.options=n,this._$Cg=(r=n==null?void 0:n.isConnected)===null||r===void 0||r}get _$AU(){var e,i;return(i=(e=this._$AM)===null||e===void 0?void 0:e._$AU)!==null&&i!==void 0?i:this._$Cg}get parentNode(){let e=this._$AA.parentNode;const i=this._$AM;return i!==void 0&&e.nodeType===11&&(e=i.parentNode),e}get startNode(){return this._$AA}get endNode(){return this._$AB}_$AI(e,i=this){e=kt(this,e,i),a1(e)?e===_e||e==null||e===""?(this._$AH!==_e&&this._$AR(),this._$AH=_e):e!==this._$AH&&e!==A0&&this.$(e):e._$litType$!==void 0?this.T(e):e.nodeType!==void 0?this.k(e):Ka(e)?this.S(e):this.$(e)}M(e,i=this._$AB){return this._$AA.parentNode.insertBefore(e,i)}k(e){this._$AH!==e&&(this._$AR(),this._$AH=this.M(e))}$(e){this._$AH!==_e&&a1(this._$AH)?this._$AA.nextSibling.data=e:this.k(bt.createTextNode(e)),this._$AH=e}T(e){var i;const{values:s,_$litType$:n}=e,r=typeof n=="number"?this._$AC(e):(n.el===void 0&&(n.el=c1.createElement(n.h,this.options)),n);if(((i=this._$AH)===null||i===void 0?void 0:i._$AD)===r)this._$AH.m(s);else{const l=new Ya(r,this),o=l.p(this.options);l.m(s),this.k(o),this._$AH=l}}_$AC(e){let i=d2.get(e.strings);return i===void 0&&d2.set(e.strings,i=new c1(e)),i}S(e){dl(this._$AH)||(this._$AH=[],this._$AR());const i=this._$AH;let s,n=0;for(const r of e)n===i.length?i.push(s=new x1(this.M(o1()),this.M(o1()),this,this.options)):s=i[n],s._$AI(r),n++;n<i.length&&(this._$AR(s&&s._$AB.nextSibling,n),i.length=n)}_$AR(e=this._$AA.nextSibling,i){var s;for((s=this._$AP)===null||s===void 0||s.call(this,!1,!0,i);e&&e!==this._$AB;){const n=e.nextSibling;e.remove(),e=n}}setConnected(e){var i;this._$AM===void 0&&(this._$Cg=e,(i=this._$AP)===null||i===void 0||i.call(this,e))}}class xi{constructor(e,i,s,n,r){this.type=1,this._$AH=_e,this._$AN=void 0,this.element=e,this.name=i,this._$AM=n,this.options=r,s.length>2||s[0]!==""||s[1]!==""?(this._$AH=Array(s.length-1).fill(new String),this.strings=s):this._$AH=_e}get tagName(){return this.element.tagName}get _$AU(){return this._$AM._$AU}_$AI(e,i=this,s,n){const r=this.strings;let l=!1;if(r===void 0)e=kt(this,e,i,0),l=!a1(e)||e!==this._$AH&&e!==A0,l&&(this._$AH=e);else{const o=e;let a,c;for(e=r[0],a=0;a<r.length-1;a++)c=kt(this,o[s+a],i,a),c===A0&&(c=this._$AH[a]),l||(l=!a1(c)||c!==this._$AH[a]),c===_e?e=_e:e!==_e&&(e+=(c!=null?c:"")+r[a+1]),this._$AH[a]=c}l&&!n&&this.C(e)}C(e){e===_e?this.element.removeAttribute(this.name):this.element.setAttribute(this.name,e!=null?e:"")}}class Za extends xi{constructor(){super(...arguments),this.type=3}C(e){this.element[this.name]=e===_e?void 0:e}}const Ja=wt?wt.emptyScript:"";class Qa extends xi{constructor(){super(...arguments),this.type=4}C(e){e&&e!==_e?this.element.setAttribute(this.name,Ja):this.element.removeAttribute(this.name)}}class e3 extends xi{constructor(e,i,s,n,r){super(e,i,s,n,r),this.type=5}_$AI(e,i=this){var s;if((e=(s=kt(this,e,i,0))!==null&&s!==void 0?s:_e)===A0)return;const n=this._$AH,r=e===_e&&n!==_e||e.capture!==n.capture||e.once!==n.once||e.passive!==n.passive,l=e!==_e&&(n===_e||r);r&&this.element.removeEventListener(this.name,this,n),l&&this.element.addEventListener(this.name,this,e),this._$AH=e}handleEvent(e){var i,s;typeof this._$AH=="function"?this._$AH.call((s=(i=this.options)===null||i===void 0?void 0:i.host)!==null&&s!==void 0?s:this.element,e):this._$AH.handleEvent(e)}}class t3{constructor(e,i,s){this.element=e,this.type=6,this._$AN=void 0,this._$AM=i,this.options=s}get _$AU(){return this._$AM._$AU}_$AI(e){kt(this,e)}}const p2=window.litHtmlPolyfillSupport;p2==null||p2(c1,x1),((Bi=globalThis.litHtmlVersions)!==null&&Bi!==void 0?Bi:globalThis.litHtmlVersions=[]).push("2.2.6");/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */var Ni,Li;class L extends dt{constructor(){super(...arguments),this.renderOptions={host:this},this._$Dt=void 0}createRenderRoot(){var e,i;const s=super.createRenderRoot();return(e=(i=this.renderOptions).renderBefore)!==null&&e!==void 0||(i.renderBefore=s.firstChild),s}update(e){const i=this.render();this.hasUpdated||(this.renderOptions.isConnected=this.isConnected),super.update(e),this._$Dt=Xa(i,this.renderRoot,this.renderOptions)}connectedCallback(){var e;super.connectedCallback(),(e=this._$Dt)===null||e===void 0||e.setConnected(!0)}disconnectedCallback(){var e;super.disconnectedCallback(),(e=this._$Dt)===null||e===void 0||e.setConnected(!1)}render(){return A0}}L.finalized=!0,L._$litElement$=!0,(Ni=globalThis.litElementHydrateSupport)===null||Ni===void 0||Ni.call(globalThis,{LitElement:L});const u2=globalThis.litElementPolyfillSupport;u2==null||u2({LitElement:L});((Li=globalThis.litElementVersions)!==null&&Li!==void 0?Li:globalThis.litElementVersions=[]).push("3.2.0");/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const U=t=>e=>typeof e=="function"?((i,s)=>(window.customElements.define(i,s),s))(t,e):((i,s)=>{const{kind:n,elements:r}=s;return{kind:n,elements:r,finisher(l){window.customElements.define(i,l)}}})(t,e);/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const i3=(t,e)=>e.kind==="method"&&e.descriptor&&!("value"in e.descriptor)?Ft(j0({},e),{finisher(i){i.createProperty(e.key,t)}}):{kind:"field",key:Symbol(),placement:"own",descriptor:{},originalKey:e.key,initializer(){typeof e.initializer=="function"&&(this[e.key]=e.initializer.call(this))},finisher(i){i.createProperty(e.key,t)}};function y(t){return(e,i)=>i!==void 0?((s,n,r)=>{n.constructor.createProperty(r,s)})(t,e,i):i3(t,e)}/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const s3=({finisher:t,descriptor:e})=>(i,s)=>{var n;if(s===void 0){const r=(n=i.originalKey)!==null&&n!==void 0?n:i.key,l=e!=null?{kind:"method",placement:"prototype",key:r,descriptor:e(i.key)}:Ft(j0({},i),{key:r});return t!=null&&(l.finisher=function(o){t(o,r)}),l}{const r=i.constructor;e!==void 0&&Object.defineProperty(i,s,e(s)),t==null||t(r,s)}};/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */function v1(t,e){return s3({descriptor:i=>{const s={get(){var n,r;return(r=(n=this.renderRoot)===null||n===void 0?void 0:n.querySelector(t))!==null&&r!==void 0?r:null},enumerable:!0,configurable:!0};if(e){const n=typeof i=="symbol"?Symbol():"__"+i;s.get=function(){var r,l;return this[n]===void 0&&(this[n]=(l=(r=this.renderRoot)===null||r===void 0?void 0:r.querySelector(t))!==null&&l!==void 0?l:null),this[n]}}return s}})}/**
 * @license
 * Copyright 2021 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */var Hi;((Hi=window.HTMLSlotElement)===null||Hi===void 0?void 0:Hi.prototype.assignedElements)!=null;const Qe=3.78;var g0=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let c0=class extends L{constructor(){super(...arguments);this.color="red",this.offColor="#444",this.background="black",this.digits=1,this.colon=!1,this.colonValue=!1,this.pins="top",this.values=[0,0,0,0,0,0,0,0]}get pinInfo(){const e=i=>{const{startX:s,cols:n,bottomY:r}=this.pinPositions,l=(i-1)%n,o=1-Math.floor((i-1)/n),c=s+1.27+(o?l:n-l-1)*2.54,h=this.pins==="top"?o?r+1:1:o?r+2:0;return{number:i,x:c*Qe,y:h*Qe}};switch(this.digits){case 4:return[Object.assign(Object.assign({name:"A"},e(13)),{signals:[],description:"Segment A"}),Object.assign(Object.assign({name:"B"},e(9)),{signals:[],description:"Segment B"}),Object.assign(Object.assign({name:"C"},e(4)),{signals:[],description:"Segment C"}),Object.assign(Object.assign({name:"D"},e(2)),{signals:[],description:"Segment D"}),Object.assign(Object.assign({name:"E"},e(1)),{signals:[],description:"Segment E"}),Object.assign(Object.assign({name:"F"},e(12)),{signals:[],description:"Segment F"}),Object.assign(Object.assign({name:"G"},e(5)),{signals:[],description:"Segment G"}),Object.assign(Object.assign({name:"DP"},e(3)),{signals:[],description:"Decimal Point"}),Object.assign(Object.assign({name:"DIG1"},e(14)),{signals:[],description:"Digit 1 Common"}),Object.assign(Object.assign({name:"DIG2"},e(11)),{signals:[],description:"Digit 2 Common"}),Object.assign(Object.assign({name:"DIG3"},e(10)),{signals:[],description:"Digit 3 Common"}),Object.assign(Object.assign({name:"DIG4"},e(6)),{signals:[],description:"Digit 4 Common"}),Object.assign(Object.assign({name:"COM"},e(7)),{signals:[],description:"Common pin"}),Object.assign(Object.assign({name:"CLN"},e(8)),{signals:[],description:"Colon"})];case 2:return[Object.assign(Object.assign({name:"DIG1"},e(8)),{signals:[],description:"Digit 1 Common"}),Object.assign(Object.assign({name:"DIG2"},e(7)),{signals:[],description:"Digit 2 Common"}),Object.assign(Object.assign({name:"A"},e(10)),{signals:[],description:"Segment A"}),Object.assign(Object.assign({name:"B"},e(9)),{signals:[],description:"Segment B"}),Object.assign(Object.assign({name:"C"},e(1)),{signals:[],description:"Segment C"}),Object.assign(Object.assign({name:"D"},e(4)),{signals:[],description:"Segment D"}),Object.assign(Object.assign({name:"E"},e(3)),{signals:[],description:"Segment E"}),Object.assign(Object.assign({name:"F"},e(6)),{signals:[],description:"Segment F"}),Object.assign(Object.assign({name:"G"},e(5)),{signals:[],description:"Segment G"}),Object.assign(Object.assign({name:"DP"},e(2)),{signals:[],description:"Decimal Point"})];case 1:default:return[Object.assign(Object.assign({name:"COM.1"},e(3)),{signals:[],description:"Common"}),Object.assign(Object.assign({name:"COM.2"},e(8)),{signals:[],description:"Common"}),Object.assign(Object.assign({name:"A"},e(7)),{signals:[],description:"Segment A"}),Object.assign(Object.assign({name:"B"},e(6)),{signals:[],description:"Segment B"}),Object.assign(Object.assign({name:"C"},e(4)),{signals:[],description:"Segment C"}),Object.assign(Object.assign({name:"D"},e(2)),{signals:[],description:"Segment D"}),Object.assign(Object.assign({name:"E"},e(1)),{signals:[],description:"Segment E"}),Object.assign(Object.assign({name:"F"},e(9)),{signals:[],description:"Segment F"}),Object.assign(Object.assign({name:"G"},e(10)),{signals:[],description:"Segment G"}),Object.assign(Object.assign({name:"DP"},e(5)),{signals:[],description:"Decimal Point"})]}}static get styles(){return ke`
      polygon {
        transform: scale(0.9);
        transform-origin: 50% 50%;
        transform-box: fill-box;
      }
    `}get pinPositions(){const{digits:e}=this,s=Math.ceil((e===4?14:10)/2);return{startX:(12.55*e-s*2.54)/2,bottomY:this.pins==="extend"?21:17,cols:s}}get yOffset(){return this.pins==="extend"?2:0}renderDigit(e,i){const s=n=>this.values[i+n]?this.color:this.offColor;return V`
      <g transform="skewX(-8) translate(${e}, ${this.yOffset+2.4}) scale(0.81)">
        <polygon points="2 0 8 0 9 1 8 2 2 2 1 1" fill="${s(0)}" />
        <polygon points="10 2 10 8 9 9 8 8 8 2 9 1" fill="${s(1)}" />
        <polygon points="10 10 10 16 9 17 8 16 8 10 9 9" fill="${s(2)}" />
        <polygon points="8 18 2 18 1 17 2 16 8 16 9 17" fill="${s(3)}" />
        <polygon points="0 16 0 10 1 9 2 10 2 16 1 17" fill="${s(4)}" />
        <polygon points="0 8 0 2 1 1 2 2 2 8 1 9" fill=${s(5)} />
        <polygon points="2 8 8 8 9 9 8 10 2 10 1 9" fill=${s(6)} />
      </g>
      <circle cx="${e+7.4}" cy="${this.yOffset+16}" r="0.89" fill="${s(7)}" />
    `}renderColon(){const{yOffset:e}=this,i=1.5+12.7*Math.round(this.digits/2),s=this.colonValue?this.color:this.offColor;return V`
      <g transform="skewX(-8)"  fill="${s}">
        <circle cx="${i}" cy="${e+5.75}" r="0.89" />
        <circle cx="${i}" cy="${e+13.25}" r="0.89" />
      </g>
    `}renderPins(){const{cols:e,bottomY:i,startX:s}=this.pinPositions;return V`
      <g fill="url(#pin-pattern)" transform="translate(${s}, 0)">
        <rect height="2" width=${e*2.54} />
        <rect height="2" width=${e*2.54} transform="translate(0, ${i})" />
      </g>
    `}render(){const{digits:e,colon:i,pins:s,yOffset:n}=this,r=12.55*e,l=s==="extend"?23:19,o=[];for(let a=0;a<e;a++)o.push(this.renderDigit(3.5+a*12.7,a*8));return G`
      <svg
        width="${r}mm"
        height="${l}mm"
        version="1.1"
        viewBox="0 0 ${r} ${l}"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="2" width="2.54" patternUnits="userSpaceOnUse">
            ${s==="extend"?V`<rect x="1.02" y="0" height="2" width="0.5" fill="#aaa" />`:V`<circle cx="1.27" cy="1" r=0.5 fill="#aaa" />`}
          </pattern>
        </defs>
        <rect x="0" y="${n}" width="${r}" height="19" />
        ${o}<!-- -->
        ${i?this.renderColon():null}<!-- -->
        ${s!=="none"?this.renderPins():null}
      </svg>
    `}};g0([y()],c0.prototype,"color",void 0);g0([y()],c0.prototype,"offColor",void 0);g0([y()],c0.prototype,"background",void 0);g0([y({type:Number})],c0.prototype,"digits",void 0);g0([y({type:Boolean})],c0.prototype,"colon",void 0);g0([y({type:Boolean})],c0.prototype,"colonValue",void 0);g0([y()],c0.prototype,"pins",void 0);g0([y({type:Array})],c0.prototype,"values",void 0);c0=g0([U("wokwi-7segment")],c0);const vi=V`
  <pattern id="pins-female" width="2.54" height="2.54" patternUnits="userSpaceOnUse">
    <rect x="0" y="0" width="2.54" height="2.54" fill="#333"></rect>
    <rect x="1.079" y="0.896" width="0.762" height="0.762" style="fill: #191919"></rect>
    <path
      transform="translate(1.079, 1.658) rotate(180 0 0)"
      d="m 0 0 v 0.762 l 0.433,0.433 c 0.046,-0.046 0.074,-0.109 0.074,-0.179 v -1.27 c 0,-0.070 -0.028,-0.133 -0.074,-0.179 z"
      style="opacity: 0.25"
    ></path>
    <path
      transform="translate(1.841, 1.658) rotate(90 0 0)"
      d="m 0 0 v 0.762 l 0.433,0.433 c 0.046,-0.046 0.074,-0.109 0.074,-0.179 v -1.27 c 0,-0.070 -0.028,-0.133 -0.074,-0.179 z"
      style="opacity: 0.3; fill: #fff"
    ></path>
    <path
      transform="translate(1.841, 0.896)"
      d="m 0 0 v 0.762 l 0.433,0.433 c 0.046,-0.046 0.074,-0.109 0.074,-0.179 v -1.27 c 0,-0.070 -0.028,-0.133 -0.074,-0.179 z"
      style="opacity: 0.15; fill: #fff"
    ></path>
    <path
      transform="translate(1.079, 0.896) rotate(270 0 0)"
      d="m 0 0 v 0.762 l 0.433,0.433 c 0.046,-0.046 0.074,-0.109 0.074,-0.179 v -1.27 c 0,-0.070 -0.028,-0.133 -0.074,-0.179 z"
      style="opacity: 0.35"
    ></path>
  </pattern>
`,H=t=>({type:"analog",channel:t}),ve=(t,e=0)=>({type:"i2c",signal:t,bus:e}),te=(t,e=0)=>({type:"spi",signal:t,bus:e}),ze=(t,e=0)=>({type:"usart",signal:t,bus:e}),$e=()=>({type:"power",signal:"GND"}),De=t=>({type:"power",signal:"VCC",voltage:t}),Te=[" ","Spacebar"];var et=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let O0=class extends L{constructor(){super(...arguments);this.led13=!1,this.ledRX=!1,this.ledTX=!1,this.ledPower=!1,this.resetPressed=!1,this.pinInfo=[{name:"A5.2",x:87,y:9,signals:[H(5),ve("SCL")]},{name:"A4.2",x:97,y:9,signals:[H(4),ve("SDA")]},{name:"AREF",x:106,y:9,signals:[]},{name:"GND.1",x:115.5,y:9,signals:[{type:"power",signal:"GND"}]},{name:"13",x:125,y:9,signals:[te("SCK")]},{name:"12",x:134.5,y:9,signals:[te("MISO")]},{name:"11",x:144,y:9,signals:[te("MOSI"),{type:"pwm"}]},{name:"10",x:153.5,y:9,signals:[te("SS"),{type:"pwm"}]},{name:"9",x:163,y:9,signals:[{type:"pwm"}]},{name:"8",x:173,y:9,signals:[]},{name:"7",x:189,y:9,signals:[]},{name:"6",x:198.5,y:9,signals:[{type:"pwm"}]},{name:"5",x:208,y:9,signals:[{type:"pwm"}]},{name:"4",x:217.5,y:9,signals:[]},{name:"3",x:227,y:9,signals:[{type:"pwm"}]},{name:"2",x:236.5,y:9,signals:[]},{name:"1",x:246,y:9,signals:[ze("TX")]},{name:"0",x:255.5,y:9,signals:[ze("RX")]},{name:"IOREF",x:131,y:191.5,signals:[]},{name:"RESET",x:140.5,y:191.5,signals:[]},{name:"3.3V",x:150,y:191.5,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"5V",x:160,y:191.5,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"GND.2",x:169.5,y:191.5,signals:[{type:"power",signal:"GND"}]},{name:"GND.3",x:179,y:191.5,signals:[{type:"power",signal:"GND"}]},{name:"VIN",x:188.5,y:191.5,signals:[{type:"power",signal:"VCC"}]},{name:"A0",x:208,y:191.5,signals:[H(0)]},{name:"A1",x:217.5,y:191.5,signals:[H(1)]},{name:"A2",x:227,y:191.5,signals:[H(2)]},{name:"A3",x:236.5,y:191.5,signals:[H(3)]},{name:"A4",x:246,y:191.5,signals:[H(4),ve("SDA")]},{name:"A5",x:255.5,y:191.5,signals:[H(5),ve("SCL")]}]}static get styles(){return ke`
      text {
        font-size: 2px;
        font-family: monospace;
        user-select: none;
      }

      circle[tabindex]:hover,
      circle[tabindex]:focus {
        stroke: white;
        outline: none;
      }
    `}render(){const{ledPower:e,led13:i,ledRX:s,ledTX:n}=this;return G`
      <svg
        width="72.58mm"
        height="53.34mm"
        version="1.1"
        viewBox="-4 0 72.58 53.34"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <g id="led-body" fill="#eee">
            <rect x="0" y="0" height="1.2" width="2.6" fill="#c6c6c6" />
            <rect x="0.6" y="-0.1" width="1.35" height="1.4" stroke="#aaa" stroke-width="0.05" />
          </g>
        </defs>

        <filter id="ledFilter" x="-0.8" y="-0.8" height="2.2" width="2.8">
          <feGaussianBlur stdDeviation="0.5" />
        </filter>

        ${vi}

        <pattern id="pin-male" width="2.54" height="4.80" patternUnits="userSpaceOnUse">
          <rect ry="0.3" rx="0.3" width="2.12" height="4.80" fill="#565656" />
          <ellipse cx="1" cy="1.13" rx="0.5" ry="0.5" fill="#aaa"></ellipse>
          <ellipse cx="1" cy="3.67" rx="0.5" ry="0.5" fill="#aaa"></ellipse>
        </pattern>

        <pattern id="mcu-leads" width="2.54" height="0.508" patternUnits="userSpaceOnUse">
          <path
            d="M 0.254,0 C 0.114,0 0,0.114 0,0.254 v 0 c 0,0.139 0,0.253 0,0.253 h 1.523 c 0,0 0,-0.114 0,-0.253 v 0 C 1.523,0.114 1.409,0 1.269,0 Z"
            fill="#ddd"
          />
        </pattern>

        <!-- PCB -->
        <path
          d="m0.999 0a1 1 0 0 0-0.999 0.999v51.34a1 1 0 0 0 0.999 0.999h64.04a1 1 0 0 0 0.999-0.999v-1.54l2.539-2.539v-32.766l-2.539-2.539v-11.43l-1.524-1.523zm14.078 0.835h0.325l0.212 0.041h0l0.105 0.021 0.300 0.124 0.270 0.180 0.229 0.229 0.180 0.270 0.017 0.042 0.097 0.234 0.01 0.023 0.050 0.252 0.013 0.066v0.325l-0.063 0.318-0.040 0.097-0.083 0.202-0 0.001-0.180 0.270-0.229 0.229-0.270 0.180-0.300 0.124-0.106 0.020-0.212 0.042h-0.325l-0.212-0.042-0.106-0.020-0.300-0.124-0.270-0.180-0.229-0.229-0.180-0.270-0 -0.001-0.083-0.202-0.040-0.097-0.063-0.318v-0.325l0.013-0.066 0.050-0.252 0.01-0.023 0.097-0.234 0.017-0.042 0.180-0.270 0.229-0.229 0.270-0.180 0.300-0.124 0.105-0.021zm50.799 15.239h0.325l0.212 0.042 0.105 0.021 0.300 0.124 0.270 0.180 0.229 0.229 0.180 0.270 0.014 0.035 0.110 0.264 0.01 0.051 0.053 0.267v0.325l-0.03 0.152-0.033 0.166-0.037 0.089-0.079 0.191-0 0.020-0.180 0.270-0.229 0.229-0.270 0.180-0.071 0.029-0.228 0.094-0.106 0.021-0.212 0.042h-0.325l-0.212-0.042-0.106-0.021-0.228-0.094-0.071-0.029-0.270-0.180-0.229-0.229-0.180-0.270-0 -0.020-0.079-0.191-0.036-0.089-0.033-0.166-0.030-0.152v-0.325l0.053-0.267 0.010-0.051 0.109-0.264 0.014-0.035 0.180-0.270 0.229-0.229 0.270-0.180 0.300-0.124 0.105-0.021zm0 27.94h0.325l0.180 0.036 0.138 0.027 0.212 0.087 0.058 0.024 0.029 0.012 0.270 0.180 0.229 0.229 0.180 0.270 0.124 0.300 0.063 0.319v0.325l-0.063 0.318-0.124 0.300-0.180 0.270-0.229 0.229-0.270 0.180-0.300 0.124-0.106 0.021-0.212 0.042h-0.325l-0.212-0.042-0.105-0.021-0.300-0.124-0.270-0.180-0.229-0.229-0.180-0.270-0.124-0.300-0.063-0.318v-0.325l0.063-0.319 0.124-0.300 0.180-0.270 0.229-0.229 0.270-0.180 0.029-0.012 0.058-0.024 0.212-0.087 0.137-0.027zm-52.07 5.080h0.325l0.212 0.041 0.106 0.021 0.300 0.124 0.270 0.180 0.229 0.229 0.121 0.182 0.058 0.087h0l0.114 0.275 0.01 0.023 0.063 0.318v0.325l-0.035 0.179-0.027 0.139-0.01 0.023-0.114 0.275h-0l-0.180 0.270-0.229 0.229-0.270 0.180-0.300 0.124-0.106 0.020-0.212 0.042h-0.325l-0.212-0.042-0.105-0.020-0.300-0.124-0.270-0.180-0.229-0.229-0.180-0.270-0.114-0.275-0.01-0.023-0.027-0.139-0.036-0.179v-0.325l0.063-0.318 0.01-0.023 0.114-0.275 0.058-0.087 0.121-0.182 0.229-0.229 0.270-0.180 0.300-0.124 0.105-0.021z"
          fill="#2b6b99"
        />

        <!-- reset button -->
        <rect x="3.816" y="1.4125" width="6.2151" height="6.0268" fill="#9b9b9b" />
        <g fill="#e6e6e6">
          <rect x="2.1368" y="1.954" width="1.695" height=".84994" />
          <rect x="2.121" y="3.8362" width="1.695" height=".84994" />
          <rect x="2.0974" y="5.8608" width="1.695" height=".84994" />
          <rect x="10.031" y="6.0256" width="1.695" height=".84994" />
          <rect x="10.008" y="1.9528" width="1.695" height=".84994" />
        </g>
        <circle
          id="reset-button"
          cx="6.9619"
          cy="4.5279"
          r="1.5405"
          fill="#960000"
          stroke="#777"
          stroke-width="0.15"
          tabindex="0"
          @mousedown=${()=>this.down()}
          @touchstart=${()=>this.down()}
          @mouseup=${()=>this.up()}
          @mouseleave=${()=>this.leave()}
          @touchend=${()=>this.leave()}
          @keydown=${r=>Te.includes(r.key)&&this.down()}
          @keyup=${r=>Te.includes(r.key)&&this.up()}
        />

        <!-- USB Connector -->
        <g style="fill:#b3b2b2;stroke:#b3b2b2;stroke-width:0.010">
          <ellipse cx="3.84" cy="9.56" rx="1.12" ry="1.03" />
          <ellipse cx="3.84" cy="21.04" rx="1.12" ry="1.03" />
          <g fill="#000">
            <rect width="11" height="11.93" x="-0.05" y="9.72" rx="0.2" ry="0.2" opacity="0.24" />
          </g>
          <rect x="-4" y="9.37" height="11.85" width="14.46" />
          <rect x="-4" y="9.61" height="11.37" width="14.05" fill="#706f6f" />
          <rect x="-4" y="9.71" height="11.17" width="13.95" fill="#9d9d9c" />
        </g>

        <!-- Power jack -->
        <g stroke-width=".254" fill="black">
          <path
            d="m-2.58 48.53v2.289c0 0.279 0.228 0.508 0.508 0.508h1.722c0.279 0 0.508-0.228 0.508-0.508v-2.289z"
            fill="#252728"
            opacity=".3"
          />
          <path
            d="m11.334 42.946c0-0.558-0.509-1.016-1.132-1.016h-10.043v9.652h10.043c0.622 0 1.132-0.457 1.132-1.016z"
            opacity=".3"
          />
          <path
            d="m-2.072 40.914c-0.279 0-0.507 0.204-0.507 0.454v8.435c0 0.279 0.228 0.507 0.507 0.507h1.722c0.279 0 0.507-0.228 0.507-0.507v-8.435c0-0.249-0.228-0.454-0.507-0.454z"
          />
          <path
            d="m-2.58 48.784v1.019c0 0.279 0.228 0.508 0.508 0.508h1.722c0.279 0 0.508-0.228 0.508-0.508v-1.019z"
            opacity=".3"
          />
          <path
            d="m11.334 43.327c0.139 0 0.254 0.114 0.254 0.254v4.064c0 0.139-0.114 0.254-0.254 0.254"
          />
          <path
            d="m11.334 42.438c0-0.558-0.457-1.016-1.016-1.016h-10.16v8.382h10.16c0.558 0 1.016-0.457 1.016-1.016z"
          />
          <path
            d="m10.064 49.804h-9.906v-8.382h1.880c-1.107 0-1.363 1.825-1.363 3.826 0 1.765 1.147 3.496 3.014 3.496h6.374z"
            opacity=".3"
          />
          <rect x="10.064" y="41.422" width=".254" height="8.382" fill="#ffffff" opacity=".2" />
          <path
            d="m10.318 48.744v1.059c0.558 0 1.016-0.457 1.016-1.016v-0.364c0 0.313-1.016 0.320-1.016 0.320z"
            opacity=".3"
          />
        </g>

        <!-- Pin Headers -->
        <g transform="translate(17.497 1.27)">
          <rect width="${.38+2.54*10}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(44.421 1.27)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(26.641 49.53)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(49.501 49.53)">
          <rect width="${.38+2.54*6}" height="2.54" fill="url(#pins-female)"></rect>
        </g>

        <!-- MCU -->
        <g>
          <path
            d="m64.932 41.627h-36.72c-0.209 0-0.379-0.170-0.379-0.379v-8.545c0-0.209 0.170-0.379 0.379-0.379h36.72c0.209 0 0.379 0.170 0.379 0.379v8.545c0 0.209-0.169 0.379-0.379 0.379z"
            fill="#292c2d"
          />
          <path
            d="m65.019 40.397c0 0.279-0.228 0.508-0.508 0.508h-35.879c-0.279 0-0.507 0.025-0.507-0.254v-6.338c0-0.279 0.228-0.508 0.507-0.508h35.879c0.279 0 0.508 0.228 0.508 0.508z"
            opacity=".3"
          />
          <path
            d="m65.019 40.016c0 0.279-0.228 0.508-0.508 0.508h-35.879c-0.279 0-0.507 0.448-0.507-0.508v-6.084c0-0.279 0.228-0.508 0.507-0.508h35.879c0.279 0 0.508 0.228 0.508 0.508z"
            fill="#3c4042"
          />
          <rect
            transform="translate(29.205, 32.778)"
            fill="url(#mcu-leads)"
            height="0.508"
            width="35.56"
          ></rect>
          <rect
            transform="translate(29.205, 41.159) scale(1 -1)"
            fill="url(#mcu-leads)"
            height="0.508"
            width="35.56"
          ></rect>
          <g fill="#252728">
            <circle cx="33.269" cy="36.847" r="1" />
            <circle cx="59.939" cy="36.847" r="1" />
            <path d="M65 38.05a1.13 1.13 0 010-2.26v2.27z" />
          </g>
        </g>

        <!-- Programming Headers -->
        <g transform="translate(14.1 4.4)">
          <rect width="7" height="4.80" fill="url(#pin-male)" />
        </g>

        <g transform="translate(63 27.2) rotate(270 0 0)">
          <rect width="7" height="4.80" fill="url(#pin-male)" />
        </g>

        <!-- LEDs -->
        <g transform="translate(57.3, 16.21)">
          <use xlink:href="#led-body" />
          ${e&&V`<circle cx="1.3" cy="0.55" r="1.3" fill="#80ff80" filter="url(#ledFilter)" />`}
        </g>

        <text fill="#fff">
          <tspan x="60.88" y="17.5">ON</tspan>
        </text>

        <g transform="translate(26.87,11.69)">
          <use xlink:href="#led-body" />
          ${i&&V`<circle cx="1.3" cy="0.55" r="1.3" fill="#ff8080" filter="url(#ledFilter)" />`}
        </g>

        <g transform="translate(26.9, 16.2)">
          <use xlink:href="#led-body" />
          ${n&&V`<circle cx="0.975" cy="0.55" r="1.3" fill="yellow" filter="url(#ledFilter)" />`}
        </g>

        <g transform="translate(26.9, 18.5)">
          <use xlink:href="#led-body" />
          ${s&&V`<circle cx="0.975" cy="0.55" r="1.3" fill="yellow" filter="url(#ledFilter)" />`}
        </g>

        <text fill="#fff" style="text-anchor: end">
          <tspan x="26.5" y="13">L</tspan>
          <tspan x="26.5" y="17.5">TX</tspan>
          <tspan x="26.5" y="19.8">RX</tspan>
          <tspan x="26.5" y="20">&nbsp;</tspan>
        </text>

        <!-- Pin Labels -->
        <rect x="28.27" y="10.34" width="36.5" height="0.16" fill="#fff"></rect>
        <text fill="#fff" style="font-weight: 900">
          <tspan x="40.84" y="9.48">DIGITAL (PWM ~)</tspan>
        </text>
        <text
          transform="translate(22.6 4) rotate(270 0 0)"
          fill="#fff"
          style="font-size: 2px; text-anchor: end; font-family: monospace"
        >
          <tspan x="0" dy="2.54">AREF</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">13</tspan>
          <tspan x="0" dy="2.54">12</tspan>
          <tspan x="0" dy="2.54">~11</tspan>
          <tspan x="0" dy="2.54">~10</tspan>
          <tspan x="0" dy="2.54">~9</tspan>
          <tspan x="0" dy="2.54">8</tspan>
          <tspan x="0" dy="4.08">7</tspan>
          <tspan x="0" dy="2.54">~6</tspan>
          <tspan x="0" dy="2.54">~5</tspan>
          <tspan x="0" dy="2.54">4</tspan>
          <tspan x="0" dy="2.54">~3</tspan>
          <tspan x="0" dy="2.54">2</tspan>
          <tspan x="0" dy="2.54">TX→1</tspan>
          <tspan x="0" dy="2.54">RX←0</tspan>
          <tspan x="0" dy="2.54">&nbsp;</tspan>
        </text>

        <rect x="33.90" y="42.76" width="12.84" height="0.16" fill="#fff"></rect>
        <rect x="49.48" y="42.76" width="14.37" height="0.16" fill="#fff"></rect>
        <text fill="#fff" style="font-weight: 900">
          <tspan x="41" y="44.96">POWER</tspan>
          <tspan x="53.5" y="44.96">ANALOG IN</tspan>
        </text>
        <text transform="translate(29.19 49) rotate(270 0 0)" fill="#fff" style="font-weight: 700">
          <tspan x="0" dy="2.54">IOREF</tspan>
          <tspan x="0" dy="2.54">RESET</tspan>
          <tspan x="0" dy="2.54">3.3V</tspan>
          <tspan x="0" dy="2.54">5V</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">Vin</tspan>
          <tspan x="0" dy="4.54">A0</tspan>
          <tspan x="0" dy="2.54">A1</tspan>
          <tspan x="0" dy="2.54">A2</tspan>
          <tspan x="0" dy="2.54">A3</tspan>
          <tspan x="0" dy="2.54">A4</tspan>
          <tspan x="0" dy="2.54">A5</tspan>
          <tspan x="0" dy="2.54">&nbsp;</tspan>
        </text>

        <!-- Logo -->
        <path
          style="fill:none;stroke:#fff;stroke-width:1.03"
          d="m 34.21393,12.01079 c -1.66494,-0.13263 -3.06393,1.83547 -2.37559,3.36182 0.66469,1.65332 3.16984,2.10396 4.36378,0.77797 1.15382,-1.13053 1.59956,-2.86476 3.00399,-3.75901 1.43669,-0.9801 3.75169,-0.0547 4.02384,1.68886 0.27358,1.66961 -1.52477,3.29596 -3.15725,2.80101 -1.20337,-0.27199 -2.06928,-1.29866 -2.56193,-2.37788 -0.6046,-1.0328 -1.39499,-2.13327 -2.62797,-2.42367 -0.2191,-0.0497 -0.44434,-0.0693 -0.66887,-0.0691 z"
        />
        <path
          style="fill:none;stroke:#fff;stroke-width:0.56"
          d="m 39.67829,14.37519 h 1.75141 m -0.89321,-0.8757 v 1.7514 m -7.30334,-0.8757 h 2.10166"
        />
        <text x="31" y="20.2" style="font-size:2.8px;font-weight:bold;line-height:1.25;fill:#fff">
          ARDUINO
        </text>

        <rect
          style="fill:none;stroke:#fff;stroke-width:0.1;stroke-dasharray:0.1, 0.1"
          width="11"
          height="5.45"
          x="45.19"
          y="11.83"
          rx="1"
          ry="1"
        />

        <text x="46.5" y="16" style="font-size:5px; line-height:1.25" fill="#fff">UNO</text>
      </svg>
    `}down(){this.resetPressed||(this.resetPressed=!0,this.resetButton.style.stroke="#333",this.dispatchEvent(new CustomEvent("button-press",{detail:"reset"})))}up(){!this.resetPressed||(this.resetPressed=!1,this.resetButton.style.stroke="",this.dispatchEvent(new CustomEvent("button-release",{detail:"reset"})))}leave(){this.resetButton.blur(),this.up()}};et([y()],O0.prototype,"led13",void 0);et([y()],O0.prototype,"ledRX",void 0);et([y()],O0.prototype,"ledTX",void 0);et([y()],O0.prototype,"ledPower",void 0);et([y()],O0.prototype,"resetPressed",void 0);et([v1("#reset-button")],O0.prototype,"resetButton",void 0);O0=et([U("wokwi-arduino-uno")],O0);const n3=new Uint8Array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,4,0,10,10,10,0,0,0,0,0,10,10,31,10,31,10,10,0,4,30,5,14,20,15,4,0,3,19,8,4,2,25,24,0,6,9,5,2,21,9,22,0,6,4,2,0,0,0,0,0,8,4,2,2,2,4,8,0,2,4,8,8,8,4,2,0,0,4,21,14,21,4,0,0,0,4,4,31,4,4,0,0,0,0,0,0,6,4,2,0,0,0,0,31,0,0,0,0,0,0,0,0,0,6,6,0,0,16,8,4,2,1,0,0,14,17,25,21,19,17,14,0,4,6,4,4,4,4,14,0,14,17,16,8,4,2,31,0,31,8,4,8,16,17,14,0,8,12,10,9,31,8,8,0,31,1,15,16,16,17,14,0,12,2,1,15,17,17,14,0,31,17,16,8,4,4,4,0,14,17,17,14,17,17,14,0,14,17,17,30,16,8,6,0,0,6,6,0,6,6,0,0,0,6,6,0,6,4,2,0,8,4,2,1,2,4,8,0,0,0,31,0,31,0,0,0,2,4,8,16,8,4,2,0,14,17,16,8,4,0,4,0,14,17,16,22,21,21,14,0,14,17,17,17,31,17,17,0,15,17,17,15,17,17,15,0,14,17,1,1,1,17,14,0,7,9,17,17,17,9,7,0,31,1,1,15,1,1,31,0,31,1,1,15,1,1,1,0,14,17,1,29,17,17,30,0,17,17,17,31,17,17,17,0,14,4,4,4,4,4,14,0,28,8,8,8,8,9,6,0,17,9,5,3,5,9,17,0,1,1,1,1,1,1,31,0,17,27,21,21,17,17,17,0,17,17,19,21,25,17,17,0,14,17,17,17,17,17,14,0,15,17,17,15,1,1,1,0,14,17,17,17,21,9,22,0,15,17,17,15,5,9,17,0,30,1,1,14,16,16,15,0,31,4,4,4,4,4,4,0,17,17,17,17,17,17,14,0,17,17,17,17,17,10,4,0,17,17,17,21,21,21,10,0,17,17,10,4,10,17,17,0,17,17,17,10,4,4,4,0,31,16,8,4,2,1,31,0,7,1,1,1,1,1,7,0,17,10,31,4,31,4,4,0,14,8,8,8,8,8,14,0,4,10,17,0,0,0,0,0,0,0,0,0,0,0,31,0,2,4,8,0,0,0,0,0,0,0,14,16,30,17,30,0,1,1,13,19,17,17,15,0,0,0,14,1,1,17,14,0,16,16,22,25,17,17,30,0,0,0,14,17,31,1,14,0,12,18,2,7,2,2,2,0,0,30,17,17,30,16,14,0,1,1,13,19,17,17,17,0,4,0,6,4,4,4,14,0,8,0,12,8,8,9,6,0,1,1,9,5,3,5,9,0,6,4,4,4,4,4,14,0,0,0,11,21,21,17,17,0,0,0,13,19,17,17,17,0,0,0,14,17,17,17,14,0,0,0,15,17,15,1,1,0,0,0,22,25,30,16,16,0,0,0,13,19,1,1,1,0,0,0,14,1,14,16,15,0,2,2,7,2,2,18,12,0,0,0,17,17,17,25,22,0,0,0,17,17,17,10,4,0,0,0,17,21,21,21,10,0,0,0,17,10,4,10,17,0,0,0,17,17,30,16,14,0,0,0,31,8,4,2,31,0,8,4,4,2,4,4,8,0,4,4,4,4,4,4,4,0,2,4,4,8,4,4,2,0,0,4,8,31,8,4,0,0,0,4,2,31,2,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,5,7,0,28,4,4,4,0,0,0,0,0,0,0,4,4,4,7,0,0,0,0,0,1,2,4,0,0,0,0,6,6,0,0,0,0,31,16,31,16,8,4,0,0,0,31,16,12,4,2,0,0,0,8,4,6,5,4,0,0,0,4,31,17,16,12,0,0,0,31,4,4,4,31,0,0,0,8,31,12,10,9,0,0,0,2,31,18,10,2,0,0,0,0,14,8,8,31,0,0,0,15,8,15,8,15,0,0,0,0,21,21,16,12,0,0,0,0,31,0,0,0,0,31,16,20,12,4,4,2,0,16,8,4,6,5,4,4,0,4,31,17,17,16,8,4,0,0,31,4,4,4,4,31,0,8,31,8,12,10,9,8,0,2,31,18,18,18,18,9,0,4,31,4,31,4,4,4,0,0,30,18,17,16,8,6,0,2,30,9,8,8,8,4,0,0,31,16,16,16,16,31,0,10,31,10,10,8,4,2,0,0,3,16,19,16,8,7,0,0,31,16,8,4,10,17,0,2,31,18,10,2,2,28,0,0,17,17,18,16,8,6,0,0,30,18,21,24,8,6,0,8,7,4,31,4,4,2,0,0,21,21,21,16,8,4,0,14,0,31,4,4,4,2,0,2,2,2,6,10,2,2,0,4,4,31,4,4,2,1,0,0,14,0,0,0,0,31,0,0,31,16,10,4,10,1,0,4,31,8,4,14,21,4,0,8,8,8,8,8,4,2,0,0,4,8,17,17,17,17,0,1,1,31,1,1,1,30,0,0,31,16,16,16,8,6,0,0,2,5,8,16,16,0,0,4,31,4,4,21,21,4,0,0,31,16,16,10,4,8,0,0,14,0,14,0,14,16,0,0,4,2,1,17,31,16,0,0,16,16,10,4,10,1,0,0,31,2,31,2,2,28,0,2,2,31,18,10,2,2,0,0,14,8,8,8,8,31,0,0,31,16,31,16,16,31,0,14,0,31,16,16,8,4,0,9,9,9,9,8,4,2,0,0,4,5,5,21,21,13,0,0,1,1,17,9,5,3,0,0,31,17,17,17,17,31,0,0,31,17,17,16,8,4,0,0,3,0,16,16,8,7,0,4,9,2,0,0,0,0,0,7,5,7,0,0,0,0,0,0,0,18,21,9,9,22,0,10,0,14,16,30,17,30,0,0,0,14,17,15,17,15,1,0,0,14,1,6,17,14,0,0,0,17,17,17,25,23,1,0,0,30,5,9,17,14,0,0,0,12,18,17,17,15,1,0,0,30,17,17,17,30,16,0,0,28,4,4,5,2,0,0,8,11,8,0,0,0,0,8,0,12,8,8,8,8,8,0,5,2,5,0,0,0,0,0,4,14,5,21,14,4,0,2,2,7,2,7,2,30,0,14,0,13,19,17,17,17,0,10,0,14,17,17,17,14,0,0,0,13,19,17,17,15,1,0,0,22,25,17,17,30,16,0,14,17,31,17,17,14,0,0,0,0,26,21,11,0,0,0,0,14,17,17,10,27,0,10,0,17,17,17,17,25,22,31,1,2,4,2,1,31,0,0,0,31,10,10,10,25,0,31,0,17,10,4,10,17,0,0,0,17,17,17,17,30,16,0,16,15,4,31,4,4,0,0,0,31,2,30,18,17,0,0,0,31,21,31,17,17,0,0,4,0,31,0,4,0,0,0,0,0,0,0,0,0,0,31,31,31,31,31,31,31,31]);var qe=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const m2=3.55,g2=5.95,Ui={green:"#6cb201",blue:"#000eff"};let Fe=class extends L{constructor(){super(...arguments);this.color="black",this.background="green",this.characters=new Uint8Array(32),this.font=n3,this.cursor=!1,this.blink=!1,this.cursorX=0,this.cursorY=0,this.backlight=!0,this.pins="full",this.numCols=16,this.numRows=2}get text(){return Array.from(this.characters).map(e=>String.fromCharCode(e)).join("")}set text(e){this.characters=new Uint8Array(e.split("").map(i=>i.charCodeAt(0)))}static get styles(){return ke`
      .cursor-blink {
        animation: cursor-blink;
      }

      @keyframes cursor-blink {
        from {
          opacity: 0;
        }
        25% {
          opacity: 1;
        }
        75% {
          opacity: 1;
        }
        to {
          opacity: 0;
        }
      }
    `}get panelHeight(){return this.rows*5.75}get pinInfo(){const{panelHeight:e}=this,i=87.5+e*Qe;switch(this.pins){case"i2c":return[{name:"GND",x:4,y:32,number:1,signals:[{type:"power",signal:"GND"}]},{name:"VCC",x:4,y:41.5,number:2,signals:[{type:"power",signal:"VCC"}]},{name:"SDA",x:4,y:51,number:3,signals:[ve("SDA")]},{name:"SCL",x:4,y:60.5,number:4,signals:[ve("SCL")]}];case"full":default:return[{name:"VSS",x:32,y:i,number:1,signals:[{type:"power",signal:"GND"}]},{name:"VDD",x:41.5,y:i,number:2,signals:[{type:"power",signal:"VCC"}]},{name:"V0",x:51.5,y:i,number:3,signals:[]},{name:"RS",x:60.5,y:i,number:4,signals:[]},{name:"RW",x:70.5,y:i,number:5,signals:[]},{name:"E",x:80,y:i,number:6,signals:[]},{name:"D0",x:89.5,y:i,number:7,signals:[]},{name:"D1",x:99.5,y:i,number:8,signals:[]},{name:"D2",x:109,y:i,number:9,signals:[]},{name:"D3",x:118.5,y:i,number:10,signals:[]},{name:"D4",x:128,y:i,number:11,signals:[]},{name:"D5",x:137.5,y:i,number:12,signals:[]},{name:"D6",x:147,y:i,number:13,signals:[]},{name:"D7",x:156.5,y:i,number:14,signals:[]},{name:"A",x:166.5,y:i,number:15,signals:[]},{name:"K",x:176,y:i,number:16,signals:[]}]}}get cols(){return this.numCols}get rows(){return this.numRows}path(e){const n=[],{cols:r}=this;for(let l=0;l<e.length;l++){const o=l%r*m2,a=Math.floor(l/r)*g2;for(let c=0;c<8;c++){const h=this.font[e[l]*8+c];for(let p=0;p<5;p++)if(h&1<<p){const d=(o+p*.6).toFixed(2),g=(a+c*.7).toFixed(2);n.push(`M ${d} ${g}h0.55v0.65h-0.55Z`)}}}return n.join(" ")}renderCursor(){const{cols:e,rows:i,cursor:s,cursorX:n,cursorY:r,blink:l,color:o}=this,a=12.45+n*m2,c=12.55+r*g2;if(n<0||n>=e||r<0||r>=i)return null;const h=[];if(l&&h.push(V`
        <rect x="${a}" y="${c}" width="2.95" height="5.55" fill="${o}">
          <animate
            attributeName="opacity"
            values="0;0;0;0;1;1;0;0;0;0"
            dur="1s"
            fill="freeze"
            repeatCount="indefinite"
          />
        </rect>
      `),s){const p=c+4.8999999999999995;h.push(V`<rect x="${a}" y="${p}" width="2.95" height="0.65" fill="${o}" />`)}return h}renderI2CPins(){return V`
      <rect x="7.55" y="-2.5" height="2.5" width="10.16" fill="url(#pins)" transform="rotate(90)" />
      <text fill="white" font-size="1.5px" font-family= "monospace">
      <tspan y="6.8" x="0.7" fill="white">1</tspan>
      <tspan y="8.9" x="2.3" fill="white">GND</tspan>
      <tspan y="11.4" x="2.3" fill="white">VCC</tspan>
      <tspan y="14" x="2.3" fill="white">SDA</tspan>
      <tspan y="16.6" x="2.3" fill="white">SCL</tspan>
      </text>
    `}renderPins(e){const i=e+21.1;return V`
      <g transform="translate(0, ${i})">
        <rect x="7.55" y="1" height="2.5" width="40.64" fill="url(#pins)" />
        <text fill="white" font-size="1.5px" font-family= "monospace">
          <tspan x="6" y="2.7">1</tspan>
          <tspan x="7.2" y="0.7">VSS</tspan>
          <tspan x="9.9" y="0.7">VDD</tspan>
          <tspan x="12.7" y="0.7">V0</tspan>
          <tspan x="15.2" y="0.7">RS</tspan>
          <tspan x="17.8" y="0.7">RW</tspan>
          <tspan x="20.8" y="0.7">E</tspan>
          <tspan x="22.7" y="0.7">D0</tspan>
          <tspan x="25.3" y="0.7">D1</tspan>
          <tspan x="27.9" y="0.7">D2</tspan>
          <tspan x="30.4" y="0.7">D3</tspan>
          <tspan x="33" y="0.7">D4</tspan>
          <tspan x="35.6" y="0.7">D5</tspan>
          <tspan x="38.2" y="0.7">D6</tspan>
          <tspan x="40.8" y="0.7">D7</tspan>
          <tspan x="43.6" y="0.7">A</tspan>
          <tspan x="46.2" y="0.7">K</tspan>
          <tspan x="48" y="2.7">16</tspan>
        </text>
      </g>
    `}render(){const{color:e,characters:i,background:s,pins:n,panelHeight:r,cols:l}=this,o=this.backlight?0:.5,a=s in Ui?Ui[s]:Ui,c=l*3.5125,h=c+23.8,p=r+24.5;return G`
      <svg
        width="${h}mm"
        height="${p}mm"
        version="1.1"
        viewBox="0 0 ${h} ${p}"
        style="font-size: 1.5px; font-family: monospace"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern
            id="characters"
            width="3.55"
            height="5.95"
            patternUnits="userSpaceOnUse"
            x="12.45"
            y="12.55"
          >
            <rect width="2.95" height="5.55" fill-opacity="0.05" />
          </pattern>
          <pattern id="pins" width="2.54" height="3.255" patternUnits="userSpaceOnUse" y="1.1">
            <path
              fill="#92926d"
              d="M0,0.55c0,0 0.21,-0.52 0.87,-0.52 0.67,0 0.81,0.51 0.81,0.51v1.81h-1.869z"
            />
            <circle r="0.45" cx="0.827" cy="0.9" color="black" />
          </pattern>
        </defs>
        <rect width="${h}" height="${p}" fill="#087f45" />
        <rect x="4.95" y="5.7" width="${c+15}" height="${r+13.7}" />
        <rect
          x="7.55"
          y="10.3"
          width="${c+9.8}"
          height="${r+4.5}"
          rx="1.5"
          ry="1.5"
          fill="${a}"
        />
        <rect
          x="7.55"
          y="10.3"
          width="${c+9.8}"
          height="${r+4.5}"
          rx="1.5"
          ry="1.5"
          opacity="${o}"
        />
        ${n==="i2c"?this.renderI2CPins():null}
        ${n==="full"?this.renderPins(r):null}
        <rect
          x="12.45"
          y="12.55"
          width="${c}"
          height="${r}"
          fill="url(#characters)"
        />
        <path d="${this.path(i)}" transform="translate(12.45, 12.55)" fill="${e}" />
        ${this.renderCursor()}
      </svg>
    `}};qe([y()],Fe.prototype,"color",void 0);qe([y()],Fe.prototype,"background",void 0);qe([y({type:Array})],Fe.prototype,"characters",void 0);qe([y()],Fe.prototype,"font",void 0);qe([y()],Fe.prototype,"cursor",void 0);qe([y()],Fe.prototype,"blink",void 0);qe([y()],Fe.prototype,"cursorX",void 0);qe([y()],Fe.prototype,"cursorY",void 0);qe([y()],Fe.prototype,"backlight",void 0);qe([y()],Fe.prototype,"pins",void 0);qe([y()],Fe.prototype,"text",null);Fe=qe([U("wokwi-lcd1602")],Fe);var tt=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const r3={red:"#ff8080",green:"#80ff80",blue:"#8080ff",yellow:"#ffff80",orange:"#ffcf80",white:"#ffffff",purple:"#ff80ff"};let R0=class extends L{constructor(){super(...arguments);this.value=!1,this.brightness=1,this.color="red",this.lightColor=null,this.label="",this.flip=!1}get pinInfo(){const e=this.flip?16:24,i=this.flip?24:16;return[{name:"A",x:e,y:42,signals:[],description:"Anode"},{name:"C",x:i,y:42,signals:[],description:"Cathode"}]}static get styles(){return ke`
      :host {
        display: inline-block;
      }

      .led-container {
        display: flex;
        flex-direction: column;
        width: 40px;
      }

      .led-label {
        font-size: 10px;
        text-align: center;
        color: gray;
        position: relative;
        line-height: 1;
        top: -8px;
      }
    `}render(){const{color:e,lightColor:i,flip:s}=this,n=i||r3[e==null?void 0:e.toLowerCase()]||e,r=this.brightness?.3+this.brightness*.7:0,l=this.value&&this.brightness>Number.EPSILON;return G`
      <div class="led-container">
        <svg
          width="40"
          height="50"
          transform="scale(${s?-1:1} 1)"
          version="1.2"
          viewBox="-10 -5 35.456 39.618"
          xmlns="http://www.w3.org/2000/svg"
        >
          <filter id="light1" x="-0.8" y="-0.8" height="2.2" width="2.8">
            <feGaussianBlur stdDeviation="2" />
          </filter>
          <filter id="light2" x="-0.8" y="-0.8" height="2.2" width="2.8">
            <feGaussianBlur stdDeviation="4" />
          </filter>
          <rect x="3.451" y="19.379" width="2.1514" height="9.8273" fill="#8c8c8c" />
          <path
            d="m12.608 29.618c0-1.1736-0.86844-2.5132-1.8916-3.4024-0.41616-0.3672-1.1995-1.0015-1.1995-1.4249v-5.4706h-2.1614v5.7802c0 1.0584 0.94752 1.8785 1.9462 2.7482 0.44424 0.37584 1.3486 1.2496 1.3486 1.7694"
            fill="#8c8c8c"
          />
          <path
            d="m14.173 13.001v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
            opacity=".3"
          />
          <path
            d="m14.173 13.001v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
            fill="#e6e6e6"
            opacity=".5"
          />
          <path
            d="m14.173 13.001v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v4.6296c1.4738 1.6517 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586l-4e-5 -1.5235c-7e-4 -1.1419-0.4744-2.2032-1.283-3.1054z"
            fill="#d1d1d1"
            opacity=".9"
          />
          <g>
            <path
              d="m14.173 13.001v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v4.6296c1.4738 1.6517 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586l-4e-5 -1.5235c-7e-4 -1.1419-0.4744-2.2032-1.283-3.1054z"
              opacity=".7"
            />
            <path
              d="m14.173 13.001v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v3.1054c1.4738 1.6502 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586-7.4e-4 -1.1412-0.47444-2.2025-1.283-3.1047z"
              opacity=".25"
            />
            <ellipse cx="7.0877" cy="16.106" rx="7.087" ry="4.9608" opacity=".25" />
          </g>
          <polygon
            points="2.2032 16.107 3.1961 16.107 3.1961 13.095 6.0156 13.095 10.012 8.8049 3.407 8.8049 2.2032 9.648"
            fill="#666666"
          />
          <polygon
            points="11.215 9.0338 7.4117 13.095 11.06 13.095 11.06 16.107 11.974 16.107 11.974 8.5241 10.778 8.5241"
            fill="#666666"
          />
          <path
            d="m14.173 13.001v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
            fill="${e}"
            opacity=".65"
          />
          <g fill="#ffffff">
            <path
              d="m10.388 3.7541 1.4364-0.2736c-0.84168-1.1318-2.0822-1.9577-3.5417-2.2385l0.25416 1.0807c0.76388 0.27072 1.4068 0.78048 1.8511 1.4314z"
              opacity=".5"
            />
            <path
              d="m0.76824 19.926v1.5199c0.64872 0.5292 1.4335 0.97632 2.3076 1.3169v-1.525c-0.8784-0.33624-1.6567-0.78194-2.3076-1.3118z"
              opacity=".5"
            />
            <path
              d="m11.073 20.21c-0.2556 0.1224-0.52992 0.22968-0.80568 0.32976-0.05832 0.01944-0.11736 0.04032-0.17784 0.05832-0.56376 0.17928-1.1614 0.31896-1.795 0.39456-0.07488 0.0094-0.1512 0.01872-0.22464 0.01944-0.3204 0.03024-0.64368 0.05832-0.97056 0.05832-0.14832 0-0.30744-0.01512-0.4716-0.02376-1.2002-0.05688-2.3306-0.31464-3.2976-0.73944l-2e-5 -8.3895v-4.8254c0-1.471 0.84816-2.7295 2.0736-3.3494l-0.02232-0.05328-1.2478-1.512c-1.6697 1.003-2.79 2.8224-2.79 4.9118v11.905c-0.04968-0.04968-0.30816-0.30888-0.48024-0.52992l-0.30744 0.6876c1.4011 1.4818 3.8088 2.4617 6.5426 2.4617 1.6798 0 3.2371-0.37368 4.5115-1.0022l-0.52704-0.40896-0.01006 0.0072z"
              opacity=".5"
            />
          </g>
          <g class="light" style="display: ${l?"":"none"}">
            <ellipse
              cx="8"
              cy="10"
              rx="10"
              ry="10"
              fill="${n}"
              filter="url(#light2)"
              style="opacity: ${r}"
            ></ellipse>
            <ellipse cx="8" cy="10" rx="2" ry="2" fill="white" filter="url(#light1)"></ellipse>
            <ellipse
              cx="8"
              cy="10"
              rx="3"
              ry="3"
              fill="white"
              filter="url(#light1)"
              style="opacity: ${r}"
            ></ellipse>
          </g>
        </svg>
        <span class="led-label">${this.label}</span>
      </div>
    `}};tt([y()],R0.prototype,"value",void 0);tt([y()],R0.prototype,"brightness",void 0);tt([y()],R0.prototype,"color",void 0);tt([y()],R0.prototype,"lightColor",void 0);tt([y()],R0.prototype,"label",void 0);tt([y({type:Boolean})],R0.prototype,"flip",void 0);R0=tt([U("wokwi-led")],R0);var wi=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let h1=class extends L{constructor(){super(...arguments);this.r=0,this.g=0,this.b=0,this.pinInfo=[{name:"VDD",y:3.5,x:0,number:1,signals:[De()]},{name:"DOUT",y:15.5,x:0,number:2,signals:[]},{name:"VSS",y:15.5,x:22,number:3,signals:[{type:"power",signal:"GND"}]},{name:"DIN",y:3.5,x:22,number:4,signals:[$e()]}]}render(){const{r:e,g:i,b:s}=this,n=C=>C>.001?.7+C*.3:0,r=Math.max(e,i,s),l=Math.min(e,i,s),o=r-l,a=Math.max(1,2-o*20),c=.1+Math.max(r*2-o*5,0),h=C=>C>.005?.1+C*.9:0,p=C=>C>.005?c+C*(1-c):0,d=C=>r?Math.floor(Math.min(h(C/r)*a,1)*255):255,g=`rgb(${d(e)}, ${d(i)}, ${d(s)})`,x=242-(r>.1&&o<.2?Math.floor(r*50*(1-o/.2)):0),A=`rgb(${x}, ${x}, ${x})`;return G`
      <svg
        width="5.6631mm"
        height="5mm"
        version="1.1"
        viewBox="0 0 5.6631 5"
        xmlns="http://www.w3.org/2000/svg"
      >
        <filter id="light1" x="-0.8" y="-0.8" height="2.8" width="2.8">
          <feGaussianBlur stdDeviation="${Math.max(.1,r)}" />
        </filter>
        <filter id="light2" x="-0.8" y="-0.8" height="2.2" width="2.8">
          <feGaussianBlur stdDeviation="0.5" />
        </filter>
        <rect x=".33308" y="0" width="5" height="5" fill="${A}" />
        <rect x=".016709" y=".4279" width=".35114" height=".9" fill="#eaeaea" />
        <rect x="0" y="3.6518" width=".35114" height=".9" fill="#eaeaea" />
        <rect x="5.312" y="3.6351" width=".35114" height=".9" fill="#eaeaea" />
        <rect x="5.312" y=".3945" width=".35114" height=".9" fill="#eaeaea" />
        <circle cx="2.8331" cy="2.5" r="2.1" fill="#ddd" />
        <circle cx="2.8331" cy="2.5" r="1.7325" fill="#e6e6e6" />
        <g fill="#bfbfbf">
          <path
            d="m4.3488 3.3308s-0.0889-0.087-0.0889-0.1341c0-0.047-6e-3 -1.1533-6e-3 -1.1533s-0.0591-0.1772-0.2008-0.1772c-0.14174 0-0.81501 0.012-0.81501 0.012s-0.24805 0.024-0.23624 0.3071c0.0118 0.2835 0.032 2.0345 0.032 2.0345 0.54707-0.046 1.0487-0.3494 1.3146-0.8888z"
          />
          <path
            d="m4.34 1.6405h-1.0805s-0.24325 0.019-0.26204-0.2423l6e-3 -0.6241c0.57782 0.075 1.0332 0.3696 1.3366 0.8706z"
          />
          <path
            d="m2.7778 2.6103-0.17127 0.124-0.8091-0.012c-0.17122-0.019-0.17062-0.2078-0.17062-0.2078-1e-3 -0.3746 1e-3 -0.2831-9e-3 -0.8122l-0.31248-0.018s0.43453-0.9216 1.4786-0.9174c-1.1e-4 0.6144-4e-3 1.2289-6e-3 1.8434z"
          />
          <path
            d="m2.7808 3.0828-0.0915-0.095h-0.96857l-0.0915 0.1447-3e-3 0.1127c0 0.065-0.12108 0.08-0.12108 0.08h-0.20909c0.55906 0.9376 1.4867 0.9155 1.4867 0.9155 1e-3 -0.3845-2e-3 -0.7692-2e-3 -1.1537z"
          />
        </g>
        <path
          d="m4.053 1.8619c-0.14174 0-0.81494 0.013-0.81494 0.013s-0.24797 0.024-0.23616 0.3084c3e-3 0.077 5e-3 0.3235 9e-3 0.5514h1.247c-2e-3 -0.33-4e-3 -0.6942-4e-3 -0.6942s-0.0593-0.1781-0.20102-0.1781z"
          fill="#666"
        />
        <ellipse
          cx="2.5"
          cy="2.3"
          rx="0.3"
          ry="0.3"
          fill="red"
          opacity=${n(e)}
          filter="url(#light1)"
        ></ellipse>
        <ellipse
          cx="3.5"
          cy="3.2"
          rx="0.3"
          ry="0.3"
          fill="green"
          opacity=${n(i)}
          filter="url(#light1)"
        ></ellipse>
        <ellipse
          cx="3.3"
          cy="1.45"
          rx="0.3"
          ry="0.3"
          fill="blue"
          opacity=${n(s)}
          filter="url(#light1)"
        ></ellipse>
        <ellipse
          cx="3"
          cy="2.5"
          rx="2.2"
          ry="2.2"
          opacity="${p(r)}"
          fill="${g}"
          filter="url(#light2)"
        ></ellipse>
      </svg>
    `}};wi([y()],h1.prototype,"r",void 0);wi([y()],h1.prototype,"g",void 0);wi([y()],h1.prototype,"b",void 0);h1=wi([U("wokwi-neopixel")],h1);var bi=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let f1=class extends L{constructor(){super(...arguments);this.color="red",this.pressed=!1,this.label="",this.pinInfo=[{name:"1.l",x:2,y:9,signals:[]},{name:"2.l",x:2,y:36,signals:[]},{name:"1.r",x:65,y:9,signals:[]},{name:"2.r",x:65,y:36,signals:[]}]}static get styles(){return ke`
      :host {
        display: inline-flex;
        flex-direction: column;
      }

      button {
        border: none;
        background: none;
        padding: 0;
        margin: 0;
        text-decoration: none;
        -webkit-appearance: none;
        -moz-appearance: none;
      }

      button:active .button-circle {
        fill: url(#grad-down);
      }

      .clickable-element {
        cursor: pointer;
      }

      .label {
        width: 0;
        min-width: 100%;
        font-size: 12px;
        text-align: center;
        color: gray;
        position: relative;
        line-height: 1;
        top: -2px;
      }
    `}render(){const{color:e,label:i}=this,s=this.pressed?"url(#grad-down)":"url(#grad-up)";return G`
      <button
        aria-label="${i} ${e} pushbutton"
        @mousedown=${this.down}
        @mouseup=${n=>!n.ctrlKey&&this.up()}
        @touchstart=${this.down}
        @touchend=${this.up}
        @keydown=${n=>Te.includes(n.key)&&this.down()}
        @keyup=${n=>Te.includes(n.key)&&!n.ctrlKey&&this.up()}
      >
        <svg
          width="18mm"
          height="12mm"
          version="1.1"
          viewBox="-3 0 18 12"
          xmlns="http://www.w3.org/2000/svg"
          xmlns:xlink="http://www.w3.org/1999/xlink"
        >
          <defs>
            <linearGradient id="grad-up" x1="0" x2="1" y1="0" y2="1">
              <stop stop-color="#ffffff" offset="0" />
              <stop stop-color="${e}" offset="0.3" />
              <stop stop-color="${e}" offset="0.5" />
              <stop offset="1" />
            </linearGradient>
            <linearGradient
              id="grad-down"
              xlink:href="#grad-up"
              gradientTransform="rotate(180,0.5,0.5)"
            ></linearGradient>
          </defs>
          <rect x="0" y="0" width="12" height="12" rx=".44" ry=".44" fill="#464646" />
          <rect x=".75" y=".75" width="10.5" height="10.5" rx=".211" ry=".211" fill="#eaeaea" />
          <g fill="#1b1b1">
            <circle cx="1.767" cy="1.7916" r=".37" />
            <circle cx="10.161" cy="1.7916" r=".37" />
            <circle cx="10.161" cy="10.197" r=".37" />
            <circle cx="1.767" cy="10.197" r=".37" />
          </g>
          <g fill="#eaeaea">
            <path
              d="m-0.3538 1.4672c-0.058299 0-0.10523 0.0469-0.10523 0.10522v0.38698h-2.1504c-0.1166 0-0.21045 0.0938-0.21045 0.21045v0.50721c0 0.1166 0.093855 0.21045 0.21045 0.21045h2.1504v0.40101c0 0.0583 0.046928 0.10528 0.10523 0.10528h0.35723v-1.9266z"
            />
            <path
              d="m-0.35376 8.6067c-0.058299 0-0.10523 0.0469-0.10523 0.10523v0.38697h-2.1504c-0.1166 0-0.21045 0.0939-0.21045 0.21045v0.50721c0 0.1166 0.093855 0.21046 0.21045 0.21046h2.1504v0.401c0 0.0583 0.046928 0.10528 0.10523 0.10528h0.35723v-1.9266z"
            />
            <path
              d="m12.354 1.4672c0.0583 0 0.10522 0.0469 0.10523 0.10522v0.38698h2.1504c0.1166 0 0.21045 0.0938 0.21045 0.21045v0.50721c0 0.1166-0.09385 0.21045-0.21045 0.21045h-2.1504v0.40101c0 0.0583-0.04693 0.10528-0.10523 0.10528h-0.35723v-1.9266z"
            />
            <path
              d="m12.354 8.6067c0.0583 0 0.10523 0.0469 0.10523 0.10522v0.38698h2.1504c0.1166 0 0.21045 0.0938 0.21045 0.21045v0.50721c0 0.1166-0.09386 0.21045-0.21045 0.21045h-2.1504v0.40101c0 0.0583-0.04693 0.10528-0.10523 0.10528h-0.35723v-1.9266z"
            />
          </g>
          <g class="clickable-element">
            <circle class="button-circle" cx="6" cy="6" r="3.822" fill="${s}" />
            <circle
              cx="6"
              cy="6"
              r="2.9"
              fill="${e}"
              stroke="#2f2f2f"
              stroke-opacity=".47"
              stroke-width=".08"
            />
          </g>
        </svg>
      </button>
      <span class="label">${this.label}</span>
    `}down(){this.pressed||(this.pressed=!0,this.dispatchEvent(new Event("button-press")))}up(){this.pressed&&(this.pressed=!1,this.dispatchEvent(new Event("button-release")))}};bi([y()],f1.prototype,"color",void 0);bi([y()],f1.prototype,"pressed",void 0);bi([y()],f1.prototype,"label",void 0);f1=bi([U("wokwi-pushbutton")],f1);var ml=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const Vi={[-2]:"#C3C7C0",[-1]:"#F1D863",0:"#000000",1:"#8F4814",2:"#FB0000",3:"#FC9700",4:"#FCF800",5:"#00B800",6:"#0000FF",7:"#A803D6",8:"#808080",9:"#FCFCFC"};let ys=class extends L{constructor(){super(...arguments);this.value="1000",this.pinInfo=[{name:"1",x:0,y:9,signals:[]},{name:"2",x:59,y:9,signals:[]}]}breakValue(e){const i=e>=1e10?9:e>=1e9?8:e>=1e8?7:e>=1e7?6:e>=1e6?5:e>=1e5?4:e>=1e4?3:e>=1e3?2:e>=100?1:e>=10?0:e>=1?-1:-2,s=Math.round(e/10**i);return e===0?[0,0]:[Math.round(s%100),i]}render(){const{value:e}=this,i=parseFloat(e),[s,n]=this.breakValue(i),r=Vi[Math.floor(s/10)],l=Vi[s%10],o=Vi[n];return G`
      <svg
        width="15.645mm"
        height="3mm"
        version="1.1"
        viewBox="0 0 15.645 3"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <linearGradient
            id="a"
            x2="0"
            y1="22.332"
            y2="38.348"
            gradientTransform="matrix(.14479 0 0 .14479 -23.155 -4.0573)"
            gradientUnits="userSpaceOnUse"
            spreadMethod="reflect"
          >
            <stop stop-color="#323232" offset="0" />
            <stop stop-color="#fff" stop-opacity=".42268" offset="1" />
          </linearGradient>
        </defs>
        <rect y="1.1759" width="15.645" height=".63826" fill="#eaeaea" />
        <g stroke-width=".14479" fill="#d5b597">
          <path
            id="body"
            d="m4.6918 0c-1.0586 0-1.9185 0.67468-1.9185 1.5022 0 0.82756 0.85995 1.4978 1.9185 1.4978 0.4241 0 0.81356-0.11167 1.1312-0.29411h4.0949c0.31802 0.18313 0.71075 0.29411 1.1357 0.29411 1.0586 0 1.9185-0.67015 1.9185-1.4978 0-0.8276-0.85995-1.5022-1.9185-1.5022-0.42499 0-0.81773 0.11098-1.1357 0.29411h-4.0949c-0.31765-0.18244-0.7071-0.29411-1.1312-0.29411z"
          />
          <use xlink:href="#body" fill="url(#a)" opacity=".44886" />
          <rect x="4" y="0" width="1" height="3" fill="${r}" clip-path="url(#g)" />

          <path d="m6 0.29411v2.4117h0.96v-2.4117z" fill="${l}" />
          <path d="m7.8 0.29411v2.4117h0.96v-2.4117z" fill="${o}" />

          <rect x="10.69" y="0" width="1" height="3" fill="#F1D863" clip-path="url(#g)" />
          <clippath id="g">
            <use xlink:href="#body" />
          </clippath>
        </g>
      </svg>
    `}};ml([y()],ys.prototype,"value",void 0);ys=ml([U("wokwi-resistor")],ys);var ki=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const l3=[10.7,25,39.3,53.6],o3=[7,22,37,52];function a3(t){return!isNaN(parseFloat(t))}let d1=class extends L{constructor(){super(...arguments);this.columns="4",this.connector=!1,this.keys=["1","2","3","A","4","5","6","B","7","8","9","C","*","0","#","D"],this.pressedKeys=new Set}get pinInfo(){switch(this.columns){case"3":return[{name:"R1",x:76.5,y:338,signals:[]},{name:"R2",x:86,y:338,signals:[]},{name:"R3",x:95.75,y:338,signals:[]},{name:"R4",x:105.25,y:338,signals:[]},{name:"C1",x:115,y:338,signals:[]},{name:"C2",x:124.5,y:338,signals:[]},{name:"C3",x:134,y:338,signals:[]}];default:return[{name:"R1",x:100,y:338,signals:[]},{name:"R2",x:110,y:338,signals:[]},{name:"R3",x:119.5,y:338,signals:[]},{name:"R4",x:129,y:338,signals:[]},{name:"C1",x:138.5,y:338,signals:[]},{name:"C2",x:148,y:338,signals:[]},{name:"C3",x:157.75,y:338,signals:[]},{name:"C4",x:167.5,y:338,signals:[]}]}}renderKey(e,i){var s;const n=(s=this.keys[e*4+i])!==null&&s!==void 0?s:"",r=a3(n)?"blue-key":"red-key",l=n.toUpperCase();return V`<g
      transform="translate(${o3[i]} ${l3[e]})"
      tabindex="0"
      class=${r}
      data-key-name=${l}
      @blur=${o=>{this.up(n,o.currentTarget)}}
      @mousedown=${()=>this.down(n)}
      @mouseup=${()=>this.up(n)}
      @touchstart=${()=>this.down(n)}
      @touchend=${()=>this.up(n)}
      @keydown=${o=>Te.includes(o.key)&&this.down(n,o.currentTarget)}
      @keyup=${o=>Te.includes(o.key)&&this.up(n,o.currentTarget)}
    >
      <use xlink:href="#key" />
      <text x="5.6" y="8.1">${n}</text>
    </g>`}render(){const{connector:e}=this,i=this.columns==="4",s=15,n=2.54,r=i?70.336:70.336-s,l=i?n*8:n*7,o=76+(e?15:0);return G`
      <style>
        text {
          fill: #dfe2e5;
          user-select: none;
        }

        g[tabindex] {
          cursor: pointer;
        }

        g[tabindex]:focus,
        g[tabindex]:active {
          stroke: white;
          outline: none;
        }

        .blue-key:focus,
        .red-key:focus {
          filter: url(#shadow);
        }

        .blue-key:active,
        .blue-key.pressed {
          fill: #4e50d7;
        }

        .red-key:active,
        .red-key.pressed {
          fill: #ab040b;
        }

        g[tabindex]:focus text {
          stroke: none;
        }

        g[tabindex]:active text,
        .blue-key.pressed text,
        .red-key.pressed text {
          fill: white;
          stroke: none;
        }
      </style>

      <svg
        width="${r}mm"
        height="${o}mm"
        version="1.1"
        viewBox="0 0 ${r} ${o}"
        font-family="sans-serif"
        font-size="8.2px"
        text-anchor="middle"
        xmlns="http://www.w3.org/2000/svg"
        @keydown=${a=>this.keyStrokeDown(a.key)}
        @keyup=${a=>this.keyStrokeUp(a.key)}
      >
        <defs>
          <rect
            id="key"
            width="11.2"
            height="11"
            rx="1.4"
            ry="1.4"
            stroke="#b1b5b9"
            stroke-width=".75"
          />
          <pattern id="wires" width="2.54" height="8" patternUnits="userSpaceOnUse">
            <rect width="2.54" height="8" fill="#eee" />
            <rect x="0.77" width="1" height="6" fill="#d9d5bc" />
            <circle cx="1.27" cy="6" r="0.75" fill="#d9d5bc" />
            <rect x="0.52" y="6" width="1.5" height="2" fill="#d9d5bc" />
          </pattern>
          <pattern id="wires-marks" width="2.54" height="8" patternUnits="userSpaceOnUse">
            <rect x="0.52" y="6" width="1.5" height="2" fill="#746d41" />
          </pattern>
          ${vi}
          <filter id="shadow">
            <feDropShadow dx="0" dy="0" stdDeviation="0.5" flood-color="#ffff99" />
          </filter>
        </defs>

        <!-- Keypad outline -->
        <rect x="0" y="0" width="${r}" height="76" rx="5" ry="5" fill="#454449" />
        <rect
          x="2.78"
          y="3.25"
          width="${i?65:65-s}"
          height="68.6"
          rx="3.5"
          ry="3.5"
          fill="none"
          stroke="#b1b5b9"
          stroke-width="1"
        />

        <!-- Connector -->
        ${e?V`
            <g transform="translate(${(r-l)/2}, 76)">
              <rect width="${l}" height="8" fill="url(#wires)" />
              <rect width="10.16" height="8" fill="url(#wires-marks)" />
              <rect y="8" width="${l}" height="7" fill="#333" />
              <rect transform="translate(0, 12)" width="${l}" height="2.54" fill="url(#pins-female)" />
            </g>
          `:null}

        <!-- Blue keys -->
        <g fill="#4e90d7">
          <g>${this.renderKey(0,0)}</g>
          <g>${this.renderKey(0,1)}</g>
          <g>${this.renderKey(0,2)}</g>
          <g>${this.renderKey(1,0)}</g>
          <g>${this.renderKey(1,1)}</g>
          <g>${this.renderKey(1,2)}</g>
          <g>${this.renderKey(2,0)}</g>
          <g>${this.renderKey(2,1)}</g>
          <g>${this.renderKey(2,2)}</g>
          <g>${this.renderKey(3,1)}</g>
        </g>

        <!-- Red keys -->
        <g fill="#e94541">
          <g>${this.renderKey(3,0)}</g>
          <g>${this.renderKey(3,2)}</g>
          ${i&&V`
              <g>${this.renderKey(0,3)}</g>
              <g>${this.renderKey(1,3)}</g>
              <g>${this.renderKey(2,3)}</g>
              <g>${this.renderKey(3,3)}</g>
          `}
        </g>
      </svg>
    `}keyIndex(e){const i=this.keys.indexOf(e);return{row:Math.floor(i/4),column:i%4}}down(e,i){this.pressedKeys.has(e)||(i&&i.classList.add("pressed"),this.pressedKeys.add(e),this.dispatchEvent(new CustomEvent("button-press",{detail:Object.assign({key:e},this.keyIndex(e))})))}up(e,i){this.pressedKeys.has(e)&&(i&&i.classList.remove("pressed"),this.pressedKeys.delete(e),this.dispatchEvent(new CustomEvent("button-release",{detail:Object.assign({key:e},this.keyIndex(e))})))}keyStrokeDown(e){var i;const s=e.toUpperCase(),n=(i=this.shadowRoot)===null||i===void 0?void 0:i.querySelector(`[data-key-name="${s}"]`);n&&this.down(s,n)}keyStrokeUp(e){var i,s;const n=e.toUpperCase(),r=(i=this.shadowRoot)===null||i===void 0?void 0:i.querySelector(`[data-key-name="${n}"]`),l=(s=this.shadowRoot)===null||s===void 0?void 0:s.querySelectorAll(".pressed");e==="Shift"&&(l==null||l.forEach(o=>{const a=o.dataset.keyName;a&&this.up(a,o)})),r&&this.up(n,r)}};ki([y()],d1.prototype,"columns",void 0);ki([y()],d1.prototype,"connector",void 0);ki([y({type:Array})],d1.prototype,"keys",void 0);d1=ki([U("wokwi-membrane-keypad")],d1);/**
 * @license
 * Copyright 2017 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const gl={ATTRIBUTE:1,CHILD:2,PROPERTY:3,BOOLEAN_ATTRIBUTE:4,EVENT:5,ELEMENT:6},yl=t=>(...e)=>({_$litDirective$:t,values:e});class xl{constructor(e){}get _$AU(){return this._$AM._$AU}_$AT(e,i,s){this._$Ct=e,this._$AM=i,this._$Ci=s}_$AS(e,i){return this.update(e,i)}update(e,i){return this.render(...i)}}/**
 * @license
 * Copyright 2018 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const vl=yl(class extends xl{constructor(t){var e;if(super(t),t.type!==gl.ATTRIBUTE||t.name!=="style"||((e=t.strings)===null||e===void 0?void 0:e.length)>2)throw Error("The `styleMap` directive must be used in the `style` attribute and must be the only part in the attribute.")}render(t){return Object.keys(t).reduce((e,i)=>{const s=t[i];return s==null?e:e+`${i=i.replace(/(?:^(webkit|moz|ms|o)|)(?=[A-Z])/g,"-$&").toLowerCase()}:${s};`},"")}update(t,[e]){const{style:i}=t.element;if(this.ct===void 0){this.ct=new Set;for(const s in e)this.ct.add(s);return this.render(e)}this.ct.forEach(s=>{e[s]==null&&(this.ct.delete(s),s.includes("-")?i.removeProperty(s):i[s]="")});for(const s in e){const n=e[s];n!=null&&(this.ct.add(s),s.includes("-")?i.setProperty(s,n):i[s]=n)}return A0}}),L1=(t,e,i)=>{const s=Math.min(i,e);return Math.max(s,t)};var it=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let T0=class extends L{constructor(){super(...arguments);this.min=0,this.max=100,this.value=0,this.step=1,this.startDegree=-135,this.endDegree=135,this.center={x:0,y:0},this.pressed=!1,this.pinInfo=[{name:"GND",x:29,y:68.5,number:1,signals:[{type:"power",signal:"GND"}]},{name:"SIG",x:37,y:68.5,number:2,signals:[H(0)]},{name:"VCC",x:44.75,y:68.5,number:3,signals:[{type:"power",signal:"VCC"}]}]}static get styles(){return ke`
      #rotating {
        transform-origin: 10px 8px;
        transform: rotate(var(--knob-angle, 0deg));
      }

      svg text {
        font-size: 1px;
        line-height: 1.25;
        letter-spacing: 0px;
        word-spacing: 0px;
        fill: #ffffff;
      }
      .hide-input {
        position: absolute;
        clip: rect(0 0 0 0);
        width: 1px;
        height: 1px;
        margin: -1px;
      }
      input:focus + svg #knob {
        stroke: #ccdae3;
        filter: url(#outline);
      }
    `}mapToMinMax(e,i,s){return e*(s-i)+i}percentFromMinMax(e,i,s){return(e-i)/(s-i)}render(){const e=L1(0,1,this.percentFromMinMax(this.value,this.min,this.max)),i=(this.endDegree-this.startDegree)*e+this.startDegree;return G`
      <input
        tabindex="0"
        type="range"
        class="hide-input"
        max="${this.max}"
        min="${this.min}"
        value="${this.value}"
        step="${this.step}"
        aria-valuemin="${this.min}"
        aria-valuenow="${this.value}"
        @input="${this.onValueChange}"
      />
      <svg
        role="slider"
        width="20mm"
        height="20mm"
        version="1.1"
        viewBox="0 0 20 20"
        xmlns="http://www.w3.org/2000/svg"
        @click="${this.focusInput}"
        @mousedown=${this.down}
        @mousemove=${this.move}
        @mouseup=${this.up}
        @touchstart=${this.down}
        @touchmove=${this.move}
        @touchend=${this.up}
        style=${vl({"--knob-angle":i+"deg"})}
      >
        <defs>
          <filter id="outline">
            <feDropShadow id="glow" dx="0" dy="0" stdDeviation="0.5" flood-color="cyan" />
          </filter>
        </defs>
        <rect
          x=".15"
          y=".15"
          width="19.5"
          height="19.5"
          ry="1.23"
          fill="#045881"
          stroke="#045881"
          stroke-width=".30"
        />
        <rect x="5.4" y=".70" width="9.1" height="1.9" fill="#ccdae3" stroke-width=".15" />
        <ellipse
          id="knob"
          cx="9.91"
          cy="8.18"
          rx="7.27"
          ry="7.43"
          fill="#e4e8eb"
          stroke-width=".15"
        />
        <rect
          x="6.6"
          y="17"
          width="6.5"
          height="2"
          fill-opacity="0"
          stroke="#fff"
          stroke-width=".30"
        />
        <g stroke-width=".15">
          <text x="6.21" y="16.6">GND</text>
          <text x="8.75" y="16.63">SIG</text>
          <text x="11.25" y="16.59">VCC</text>
        </g>
        <g fill="#fff" stroke-width=".15">
          <ellipse cx="1.68" cy="1.81" rx=".99" ry=".96" />
          <ellipse cx="1.48" cy="18.37" rx=".99" ry=".96" />
          <ellipse cx="17.97" cy="18.47" rx=".99" ry=".96" />
          <ellipse cx="18.07" cy="1.91" rx=".99" ry=".96" />
        </g>
        <g fill="#b3b1b0" stroke-width=".15">
          <ellipse cx="7.68" cy="18" rx=".61" ry=".63" />
          <ellipse cx="9.75" cy="18" rx=".61" ry=".63" />
          <ellipse cx="11.87" cy="18" rx=".61" ry=".63" />
        </g>
        <ellipse cx="9.95" cy="8.06" rx="6.60" ry="6.58" fill="#c3c2c3" stroke-width=".15" />
        <rect id="rotating" x="10" y="2" width=".42" height="3.1" stroke-width=".15" />
      </svg>
    `}focusInput(){var e;const i=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector(".hide-input");i==null||i.focus()}onValueChange(e){const i=e.target;this.updateValue(parseFloat(i.value))}down(e){(e.button===0||window.navigator.maxTouchPoints)&&(this.pressed=!0,this.updatePotentiometerPosition(e))}move(e){const{pressed:i}=this;i&&this.rotateHandler(e)}up(){this.pressed=!1}updatePotentiometerPosition(e){var i,s;e.stopPropagation(),e.preventDefault();const n=(s=(i=this.shadowRoot)===null||i===void 0?void 0:i.querySelector("#knob"))===null||s===void 0?void 0:s.getBoundingClientRect();n&&(this.center={x:window.scrollX+n.left+n.width/2,y:window.scrollY+n.top+n.height/2})}rotateHandler(e){e.stopPropagation(),e.preventDefault();const i=e.type==="touchmove",s=i?e.touches[0].pageX:e.pageX,n=i?e.touches[0].pageY:e.pageY,r=this.center.x-s,l=this.center.y-n;let o=Math.round(Math.atan2(l,r)*180/Math.PI);o<0&&(o+=360),o-=90,r>0&&l<=0&&(o-=360),o=L1(this.startDegree,this.endDegree,o);const a=this.percentFromMinMax(o,this.startDegree,this.endDegree),c=this.mapToMinMax(a,this.min,this.max);this.updateValue(c)}updateValue(e){const i=L1(this.min,this.max,e),s=Math.round(i/this.step)*this.step;this.value=Math.round(s*100)/100,this.dispatchEvent(new InputEvent("input",{detail:this.value}))}};it([y()],T0.prototype,"min",void 0);it([y()],T0.prototype,"max",void 0);it([y()],T0.prototype,"value",void 0);it([y()],T0.prototype,"step",void 0);it([y()],T0.prototype,"startDegree",void 0);it([y()],T0.prototype,"endDegree",void 0);T0=it([U("wokwi-potentiometer")],T0);var st=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const D1=5.66,A1=5;let M0=class extends L{constructor(){super(...arguments);this.rows=8,this.cols=8,this.rowSpacing=1,this.colSpacing=1,this.blurLight=!1,this.animation=!1,this.pixelElements=null,this.animationFrame=null,this.animateStep=()=>{const e=new Date().getTime(),{rows:i,cols:s}=this,n=r=>r%2e3>1e3?1-r%1e3/1e3:r%1e3/1e3;for(let r=0;r<i;r++)for(let l=0;l<s;l++){const o=Math.sqrt((r-i/2+.5)**2+(l-s/2+.5)**2);this.setPixel(r,l,{r:n(o*100+e),g:n(o*100+e+200),b:n(o*100+e+400)})}this.animationFrame=requestAnimationFrame(this.animateStep)}}get pinInfo(){const{cols:e,rows:i,rowSpacing:s,colSpacing:n}=this,l=2.54*Qe,o=e*(n+D1)/2*Qe,a=i*(s+A1)*Qe;return[{name:"GND",x:o-1.5*l,y:a,signals:[$e()]},{name:"VCC",x:o-.5*l,y:a,signals:[De()]},{name:"DIN",x:o+.5*l,y:a,signals:[]},{name:"DOUT",x:o+1.5*l,y:a,signals:[]}]}static get styles(){return ke`
      :host {
        display: flex;
      }
    `}getPixelElements(){return this.shadowRoot?(this.pixelElements||(this.pixelElements=Array.from(this.shadowRoot.querySelectorAll("g.pixel")).map(e=>Array.from(e.querySelectorAll("ellipse")))),this.pixelElements):null}reset(){const e=this.getPixelElements();if(!!e)for(const[i,s,n,r]of e)i.style.opacity="0",s.style.opacity="0",n.style.opacity="0",r.style.opacity="0"}setPixel(e,i,s){const n=this.getPixelElements();if(e<0||i<0||e>=this.rows||i>=this.cols||!n)return null;const{r,g:l,b:o}=s,a=K=>K>.001?.7+K*.3:0,c=Math.max(r,l,o),h=Math.min(r,l,o),p=c-h,d=Math.max(1,2-p*20),g=.1+Math.max(c*2-p*5,0),x=K=>K>.005?.1+K*.9:0,A=K=>K>.005?g+K*(1-g):0,C=K=>c?Math.floor(Math.min(x(K/c)*d,1)*255):255,D=`rgb(${C(r)}, ${C(l)}, ${C(o)})`,z=n[e*this.cols+i],[O,P,W,J]=z;O.style.opacity=a(r).toFixed(2),P.style.opacity=a(l).toFixed(2),W.style.opacity=a(o).toFixed(2),J.style.opacity=A(c).toFixed(2),J.style.fill=D}updated(){this.animation&&!this.animationFrame?this.animationFrame=requestAnimationFrame(this.animateStep):!this.animation&&this.animationFrame&&(cancelAnimationFrame(this.animationFrame),this.animationFrame=null)}renderPixels(){const e=[],{cols:i,rows:s,colSpacing:n,rowSpacing:r}=this,l=D1+n,o=A1+r;for(let a=0;a<s;a++)for(let c=0;c<i;c++)e.push(V`
        <g transform="translate(${l*c}, ${o*a})" class="pixel">
          <ellipse cx="2.5" cy="2.3" rx="0.3" ry="0.3" fill="red" opacity="0" />
          <ellipse cx="3.5" cy="3.2" rx="0.3" ry="0.3" fill="green" opacity="0" />
          <ellipse cx="3.3" cy="1.45" rx="0.3" ry="0.3" fill="blue" opacity="0" />
          <ellipse cx="3" cy="2.5" rx="2.2" ry="2.2" opacity="0" />
        </g>`);return this.pixelElements=null,e}render(){const{cols:e,rows:i,rowSpacing:s,colSpacing:n,blurLight:r}=this,l=D1+n,o=A1+s,a=D1*e+n*(e-1),c=A1*i+s*(i-1);return G`
      <svg
        width="${a}mm"
        height="${c}mm"
        version="1.1"
        viewBox="0 0 ${a} ${c}"
        xmlns="http://www.w3.org/2000/svg"
      >
        <filter id="blurLight" x="-0.8" y="-0.8" height="2.8" width="2.8">
          <feGaussianBlur stdDeviation="0.3" />
        </filter>

        <pattern id="pixel" width="${l}" height="${o}" patternUnits="userSpaceOnUse">
          <rect x=".33308" y="0" width="5" height="5" fill="#fff" />
          <rect x=".016709" y=".4279" width=".35114" height=".9" fill="#eaeaea" />
          <rect x="0" y="3.6518" width=".35114" height=".9" fill="#eaeaea" />
          <rect x="5.312" y="3.6351" width=".35114" height=".9" fill="#eaeaea" />
          <rect x="5.312" y=".3945" width=".35114" height=".9" fill="#eaeaea" />
          <circle cx="2.8331" cy="2.5" r="2.1" fill="#ddd" />
          <circle cx="2.8331" cy="2.5" r="1.7325" fill="#e6e6e6" />
          <g fill="#bfbfbf">
            <path
              d="m4.3488 3.3308s-0.0889-0.087-0.0889-0.1341c0-0.047-6e-3 -1.1533-6e-3 -1.1533s-0.0591-0.1772-0.2008-0.1772c-0.14174 0-0.81501 0.012-0.81501 0.012s-0.24805 0.024-0.23624 0.3071c0.0118 0.2835 0.032 2.0345 0.032 2.0345 0.54707-0.046 1.0487-0.3494 1.3146-0.8888z"
            />
            <path
              d="m4.34 1.6405h-1.0805s-0.24325 0.019-0.26204-0.2423l6e-3 -0.6241c0.57782 0.075 1.0332 0.3696 1.3366 0.8706z"
            />
            <path
              d="m2.7778 2.6103-0.17127 0.124-0.8091-0.012c-0.17122-0.019-0.17062-0.2078-0.17062-0.2078-1e-3 -0.3746 1e-3 -0.2831-9e-3 -0.8122l-0.31248-0.018s0.43453-0.9216 1.4786-0.9174c-1.1e-4 0.6144-4e-3 1.2289-6e-3 1.8434z"
            />
            <path
              d="m2.7808 3.0828-0.0915-0.095h-0.96857l-0.0915 0.1447-3e-3 0.1127c0 0.065-0.12108 0.08-0.12108 0.08h-0.20909c0.55906 0.9376 1.4867 0.9155 1.4867 0.9155 1e-3 -0.3845-2e-3 -0.7692-2e-3 -1.1537z"
            />
          </g>
          <path
            d="m4.053 1.8619c-0.14174 0-0.81494 0.013-0.81494 0.013s-0.24797 0.024-0.23616 0.3084c3e-3 0.077 5e-3 0.3235 9e-3 0.5514h1.247c-2e-3 -0.33-4e-3 -0.6942-4e-3 -0.6942s-0.0593-0.1781-0.20102-0.1781z"
            fill="#666"
          />
        </pattern>
        <rect width="${a}" height="${c}" fill="url(#pixel)"></rect>
        <g style="${r?"filter: url(#blurLight)":""}">${this.renderPixels()}</g>
      </svg>
    `}};st([y()],M0.prototype,"rows",void 0);st([y()],M0.prototype,"cols",void 0);st([y({attribute:"rowspacing"})],M0.prototype,"rowSpacing",void 0);st([y({attribute:"colspacing"})],M0.prototype,"colSpacing",void 0);st([y()],M0.prototype,"blurLight",void 0);st([y()],M0.prototype,"animation",void 0);M0=st([U("wokwi-neopixel-matrix")],M0);var wl=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let xs=class extends L{constructor(){super();this.width=150,this.height=116,this.screenWidth=128,this.screenHeight=64,this.canvas=void 0,this.ctx=null,this.pinInfo=[{name:"DATA",x:36.5,y:12.5,signals:[ve("SDA")]},{name:"CLK",x:45.5,y:12.5,signals:[ve("SCL")]},{name:"DC",x:54.5,y:12.5,signals:[]},{name:"RST",x:64.5,y:12.5,signals:[]},{name:"CS",x:74.5,y:12.5,signals:[]},{name:"3V3",x:83.5,y:12.5,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"VIN",x:93.5,y:12.5,signals:[{type:"power",signal:"VCC"}]},{name:"GND",x:103.5,y:12,signals:[{type:"power",signal:"GND"}]}],this.imageData=new ImageData(this.screenWidth,this.screenHeight)}static get styles(){return ke`
      .container {
        position: relative;
      }

      .container > canvas {
        position: absolute;
        left: 11.5px;
        top: 27px;
      }

      .pixelated {
        image-rendering: crisp-edges; /* firefox */
        image-rendering: pixelated; /* chrome/webkit */
      }
    `}redraw(){var e;(e=this.ctx)===null||e===void 0||e.putImageData(this.imageData,0,0)}initContext(){var e,i;this.canvas=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector("canvas"),this.ctx=(i=this.canvas)===null||i===void 0?void 0:i.getContext("2d")}firstUpdated(){var e;this.initContext(),(e=this.ctx)===null||e===void 0||e.putImageData(this.imageData,0,0)}updated(){this.imageData&&this.redraw()}render(){const{width:e,height:i,screenWidth:s,screenHeight:n}=this;return G` <div class="container">
      <svg width="${e}" height="${i}" xmlns="http://www.w3.org/2000/svg">
        <rect stroke="#BE9B72" fill="#025CAF" x=".5" y=".5" width="148" height="114" rx="13" />

        <g transform="translate(6 6)" fill="#59340A" stroke="#BE9B72" stroke-width="0.6px">
          <circle cx="130" cy="6" r="5.5" />
          <circle cx="6" cy="6" r="5.5" />
          <circle cx="130" cy="96" r="5.5" />
          <circle cx="6" cy="96" r="5.5" />
        </g>

        <!-- 128 x 64 screen -->
        <rect x="11.4" y="26" fill="#1A1A1A" width="${s}" height="${n}" />

        <!-- All texts -->
        <text
          fill="#FFF"
          text-anchor="middle"
          font-size="5"
          font-weight="300"
          font-family="monospace"
        >
          <tspan x="37" y="8">Data</tspan>
          <tspan x="56" y="8">SA0</tspan>
          <tspan x="78" y="8">CS</tspan>
          <tspan x="97" y="8">Vin</tspan>
          <tspan x="41" y="23">C1k</tspan>
          <tspan x="53" y="23">DC</tspan>
          <tspan x="64" y="23">Rst</tspan>
          <tspan x="80" y="23">3v3</tspan>
          <tspan x="99" y="23">Gnd</tspan>
        </text>

        <!-- Star -->
        <path
          fill="#FFF"
          d="M115.5 10.06l-1.59 2.974-3.453.464 2.495 2.245-.6 3.229 3.148-1.528 3.148 1.528-.6-3.23 2.495-2.244-3.453-.464-1.59-2.974z"
          stroke="#FFF"
        />

        <!-- PINS -->
        <g transform="translate(33 9)" fill="#9D9D9A" stroke-width="0.4">
          <circle stroke="#262626" cx="70.5" cy="3.5" r="3.5" />
          <circle stroke="#007ADB" cx="60.5" cy="3.5" r="3.5" />
          <circle stroke="#9D5B96" cx="50.5" cy="3.5" r="3.5" />
          <circle stroke="#009E9B" cx="41.5" cy="3.5" r="3.5" />
          <circle stroke="#E8D977" cx="31.5" cy="3.5" r="3.5" />
          <circle stroke="#C08540" cx="21.5" cy="3.5" r="3.5" />
          <circle stroke="#B4AEAB" cx="12.5" cy="3.5" r="3.5" />
          <circle stroke="#E7DBDB" cx="3.5" cy="3.5" r="3.5" />
        </g>
      </svg>
      <canvas width="${s}" height="${n}" class="pixelated"></canvas>
    </div>`}};wl([y()],xs.prototype,"imageData",void 0);xs=wl([U("wokwi-ssd1306")],xs);var bl=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let vs=class extends L{constructor(){super(...arguments);this.hasSignal=!1,this.pinInfo=[{name:"1",x:30,y:82,signals:[]},{name:"2",x:34,y:82,signals:[]}]}static get styles(){return ke`
      :host {
        display: inline-block;
      }

      .buzzer-container {
        display: flex;
        flex-direction: column;
        width: 75px;
      }

      .music-note {
        position: relative;
        left: 40px;
        animation-duration: 1.5s;
        animation-name: animate-note;
        animation-iteration-count: infinite;
        animation-timing-function: linear;
        transform: scale(1.5);
        fill: blue;
        offset-path: path(
          'm0 0c-0.9-0.92-1.8-1.8-2.4-2.8-0.56-0.92-0.78-1.8-0.58-2.8 0.2-0.92 0.82-1.8 1.6-2.8 0.81-0.92 1.8-1.8 2.6-2.8 0.81-0.92 1.4-1.8 1.6-2.8 0.2-0.92-0.02-1.8-0.58-2.8-0.56-0.92-1.5-1.8-2.4-2.8'
        );
        offset-rotate: 0deg;
      }

      @keyframes animate-note {
        0% {
          offset-distance: 0%;
          opacity: 0;
        }
        10% {
          offset-distance: 10%;
          opacity: 1;
        }
        75% {
          offset-distance: 75%;
          opacity: 1;
        }
        100% {
          offset-distance: 100%;
          opacity: 0;
        }
      }
    `}render(){const e=this.hasSignal;return G`
      <div class="buzzer-container">
        <svg
          class="music-note"
          style="visibility: ${e?"":"hidden"}"
          xmlns="http://www.w3.org/2000/svg"
          width="8"
          height="8"
          viewBox="0 0 8 8"
        >
          <path
            d="M8 0c-5 0-6 1-6 1v4.09c-.15-.05-.33-.09-.5-.09-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5v-3.97c.73-.23 1.99-.44 4-.5v2.06c-.15-.05-.33-.09-.5-.09-.83 0-1.5.67-1.5 1.5s.67 1.5 1.5 1.5 1.5-.67 1.5-1.5v-5.5z"
          />
        </svg>

        <svg
          width="17mm"
          height="20mm"
          version="1.1"
          viewBox="0 0 17 20"
          xmlns="http://www.w3.org/2000/svg"
        >
          <path d="m8 16.5v3.5" fill="none" stroke="#000" stroke-width=".5" />
          <path d="m9 16.5v3.5" fill="#f00" stroke="#f00" stroke-width=".5" />
          <g stroke="#000">
            <g>
              <ellipse cx="8.5" cy="8.5" rx="8.15" ry="8.15" fill="#1a1a1a" stroke-width=".7" />
              <circle
                cx="8.5"
                cy="8.5"
                r="6.3472"
                fill="none"
                stroke-width=".3"
                style="paint-order:normal"
              />
              <circle
                cx="8.5"
                cy="8.5"
                r="4.3488"
                fill="none"
                stroke-width=".3"
                style="paint-order:normal"
              />
            </g>
            <circle cx="8.5" cy="8.5" r="1.3744" fill="#ccc" stroke-width=".25" />
          </g>
        </svg>
      </div>
    `}};bl([y()],vs.prototype,"hasSignal",void 0);vs=bl([U("wokwi-buzzer")],vs);/**
 * @license
 * Copyright 2018 Google LLC
 * SPDX-License-Identifier: BSD-3-Clause
 */const c3=yl(class extends xl{constructor(t){var e;if(super(t),t.type!==gl.ATTRIBUTE||t.name!=="class"||((e=t.strings)===null||e===void 0?void 0:e.length)>2)throw Error("`classMap()` can only be used in the `class` attribute and must be the only part in the attribute.")}render(t){return" "+Object.keys(t).filter(e=>t[e]).join(" ")+" "}update(t,[e]){var i,s;if(this.et===void 0){this.et=new Set,t.strings!==void 0&&(this.st=new Set(t.strings.join(" ").split(/\s/).filter(r=>r!=="")));for(const r in e)e[r]&&!(!((i=this.st)===null||i===void 0)&&i.has(r))&&this.et.add(r);return this.render(e)}const n=t.element.classList;this.et.forEach(r=>{r in e||(n.remove(r),this.et.delete(r))});for(const r in e){const l=!!e[r];l===this.et.has(r)||((s=this.st)===null||s===void 0?void 0:s.has(r))||(l?(n.add(r),this.et.add(r)):(n.remove(r),this.et.delete(r)))}return A0}});var h3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let y2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",x:122,y:286,signals:[$e()]},{name:"DIAL",x:131.6,y:286,signals:[]},{name:"PULSE",x:141.2,y:286,signals:[]}],this.digit="",this.stylesMapping={},this.classes={"rotate-path":!0},this.degrees=[320,56,87,115,143,173,204,232,260,290]}static get styles(){return ke`
      .text {
        cursor: grab;
        user-select: none;
      }
      input:focus + svg #container {
        stroke: #4e50d7;
        stroke-width: 3;
      }
      .hide-input {
        position: absolute;
        clip: rect(0 0 0 0);
        width: 1px;
        height: 1px;
        margin: -1px;
      }
      .rotate-path {
        transform-origin: 133px 133px;
        transition: transform 1000ms ease-in;
      }
      .dialer-anim {
        transform: rotate(var(--angle));
      }
    `}removeDialerAnim(){this.classes=Object.assign(Object.assign({},this.classes),{"dialer-anim":!1}),this.stylesMapping={"--angle":0},this.requestUpdate()}dial(e){this.digit=e,this.addDialerAnim(e)}onValueChange(e){const i=e.target;this.digit=parseInt(i.value),this.dial(this.digit),i.value=""}addDialerAnim(e){requestAnimationFrame(()=>{this.dispatchEvent(new CustomEvent("dial-start",{detail:{digit:this.digit}})),this.classes=Object.assign(Object.assign({},this.classes),{"dialer-anim":!0});const i=this.degrees[e];this.stylesMapping={"--angle":i+"deg"},this.requestUpdate()})}focusInput(){var e;const i=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector(".hide-input");i==null||i.focus()}render(){return G`
      <input
        tabindex="0"
        type="number"
        class="hide-input"
        value="${this.digit}"
        @input="${this.onValueChange}"
      />
      <svg width="266" height="286" @click="${this.focusInput}" xmlns="http://www.w3.org/2000/svg">
        <!-- Pins -->
        <g fill="#9f9f9f" stroke-width=".987">
          <path
            d="m123.4 266c0-0.377-0.149-0.739-0.416-1.01-0.268-0.267-0.629-0.417-1.01-0.417-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.418 0.419h2.01c0.231 0 0.418-0.188 0.418-0.419v-25.8z"
          />
          <path
            d="m133 266c0-0.377-0.149-0.739-0.416-1.01-0.268-0.267-0.629-0.417-1.01-0.417-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.418 0.419h2.01c0.231 0 0.418-0.188 0.418-0.419v-25.8z"
          />
          <path
            d="m142.6 266c0-0.377-0.15-0.739-0.417-1.01s-0.629-0.417-1.01-0.417c-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.419 0.419h2.01c0.231 0 0.419-0.188 0.419-0.419v-25.8z"
          />
        </g>
        <g transform="translate(1 1)">
          <circle stroke="#979797" stroke-width="3" fill="#1F1F1F" cx="133" cy="133" r="131" />
          <circle stroke="#fff" stroke-width="2" fill="#D8D8D8" cx="133" cy="133" r="72" />
          <path
            class=${c3(this.classes)}
            @transitionstart="${()=>{this.classes["dialer-anim"]||this.dispatchEvent(new CustomEvent("dial",{detail:{digit:this.digit}}))}}"
            @transitionend="${()=>{this.classes["dialer-anim"]||this.dispatchEvent(new CustomEvent("dial-end",{detail:{digit:this.digit}})),this.removeDialerAnim()}}"
            d="M133.5,210 C146.478692,210 157,220.521308 157,233.5 C157,246.478692 146.478692,257 133.5,257 C120.521308,257 110,246.478692 110,233.5 C110,220.521308 120.521308,210 133.5,210 Z M83.5,197 C96.4786916,197 107,207.521308 107,220.5 C107,233.478692 96.4786916,244 83.5,244 C70.5213084,244 60,233.478692 60,220.5 C60,207.521308 70.5213084,197 83.5,197 Z M45.5,163 C58.4786916,163 69,173.521308 69,186.5 C69,199.478692 58.4786916,210 45.5,210 C32.5213084,210 22,199.478692 22,186.5 C22,173.521308 32.5213084,163 45.5,163 Z M32.5,114 C45.4786916,114 56,124.521308 56,137.5 C56,150.478692 45.4786916,161 32.5,161 C19.5213084,161 9,150.478692 9,137.5 C9,124.521308 19.5213084,114 32.5,114 Z M234.5,93 C247.478692,93 258,103.521308 258,116.5 C258,129.478692 247.478692,140 234.5,140 C221.521308,140 211,129.478692 211,116.5 C211,103.521308 221.521308,93 234.5,93 Z M41.5,64 C54.4786916,64 65,74.5213084 65,87.5 C65,100.478692 54.4786916,111 41.5,111 C28.5213084,111 18,100.478692 18,87.5 C18,74.5213084 28.5213084,64 41.5,64 Z M214.5,46 C227.478692,46 238,56.5213084 238,69.5 C238,82.4786916 227.478692,93 214.5,93 C201.521308,93 191,82.4786916 191,69.5 C191,56.5213084 201.521308,46 214.5,46 Z M76.5,26 C89.4786916,26 100,36.5213084 100,49.5 C100,62.4786916 89.4786916,73 76.5,73 C63.5213084,73 53,62.4786916 53,49.5 C53,36.5213084 63.5213084,26 76.5,26 Z M173.5,15 C186.478692,15 197,25.5213084 197,38.5 C197,51.4786916 186.478692,62 173.5,62 C160.521308,62 150,51.4786916 150,38.5 C150,25.5213084 160.521308,15 173.5,15 Z M123.5,7 C136.478692,7 147,17.5213084 147,30.5 C147,43.4786916 136.478692,54 123.5,54 C110.521308,54 100,43.4786916 100,30.5 C100,17.5213084 110.521308,7 123.5,7 Z"
            id="slots"
            stroke="#fff"
            fill-opacity="0.5"
            fill="#D8D8D8"
            style=${vl(this.stylesMapping)}
          ></path>
        </g>
        <circle id="container" fill-opacity=".5" fill="#070707" cx="132.5" cy="132.5" r="132.5" />
        <g class="text" font-family="Marker Felt, monospace" font-size="21" fill="#FFF">
          <text @mouseup=${()=>this.dial(0)} x="129" y="243">0</text>
          <text @mouseup=${()=>this.dial(9)} x="78" y="230">9</text>
          <text @mouseup=${()=>this.dial(8)} x="40" y="194">8</text>
          <text @mouseup=${()=>this.dial(7)} x="28" y="145">7</text>
          <text @mouseup=${()=>this.dial(6)} x="35" y="97">6</text>
          <text @mouseup=${()=>this.dial(5)} x="72" y="58">5</text>
          <text @mouseup=${()=>this.dial(4)} x="117" y="41">4</text>
          <text @mouseup=${()=>this.dial(3)} x="168" y="47">3</text>
          <text @mouseup=${()=>this.dial(2)} x="210" y="79">2</text>
          <text @mouseup=${()=>this.dial(1)} x="230" y="126">1</text>
        </g>
        <path
          d="M182.738529,211.096297 L177.320119,238.659185 L174.670528,252.137377 L188.487742,252.137377 L182.738529,211.096297 Z"
          stroke="#979797"
          fill="#D8D8D8"
          transform="translate(181.562666, 230.360231) rotate(-22.000000) translate(-181.562666, -230.360231)"
        ></path>
      </svg>
    `}};y2=h3([U("wokwi-rotary-dialer")],y2);var zi=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let p1=class extends L{constructor(){super(...arguments);this.angle=0,this.horn="single",this.hornColor="#ccc",this.pinInfo=[{name:"GND",x:0,y:50,signals:[{type:"power",signal:"GND"}]},{name:"V+",x:0,y:59.5,signals:[{type:"power",signal:"VCC"}]},{name:"PWM",x:0,y:69,signals:[{type:"pwm"}]}]}hornPath(){switch(this.horn){case"cross":return"m119.54 50.354h-18.653v-18.653a8.4427 8.4427 0 0 0-8.4427-8.4427h-1.9537a8.4427 8.4427 0 0 0-8.4427 8.4427v18.653h-18.653a8.4427 8.4427 0 0 0-8.4427 8.4427v1.9537a8.4427 8.4427 0 0 0 8.4427 8.4427h18.653v18.653a8.4427 8.4427 0 0 0 8.4427 8.4427h1.9537a8.4427 8.4427 0 0 0 8.4427-8.4427v-18.653h18.653a8.4427 8.4427 0 0 0 8.4426-8.4427v-1.9537a8.4427 8.4427 0 0 0-8.4426-8.4427zm-57.447 12.136a2.7165 2.7165 0 1 1 2.7119-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165zm8.7543 0a2.7165 2.7165 0 1 1 2.7119-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165zm20.621-34.813a2.7165 2.7165 0 1 1-2.7165 2.7165 2.7165 2.7165 0 0 1 2.7165-2.7165zm0 8.7543a2.7165 2.7165 0 1 1-2.7165 2.7165 2.7165 2.7165 0 0 1 2.7165-2.7165zm0 55.438a2.7165 2.7165 0 1 1 2.7165-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165zm0-8.7543a2.7165 2.7165 0 1 1 2.7165-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165zm5.9215-17.42a8.3729 8.3729 0 1 1 0-11.843 8.3729 8.3729 0 0 1 0 11.843zm14.704-3.205a2.7165 2.7165 0 1 1 2.7165-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165zm8.7543 0a2.7165 2.7165 0 1 1 2.7165-2.7165 2.7165 2.7165 0 0 1-2.7165 2.7165z";case"double":return"m101.63 57.808c-0.0768-0.48377-0.16978-0.8838-0.23258-1.1629l-4.112-51.454c0-2.8654-2.6026-5.1912-5.8145-5.1912s-5.8145 2.3258-5.8145 5.1912l-4.1004 51.447c-0.07443 0.28607-0.16746 0.69774-0.24421 1.1629a12.473 12.473 0 0 0 0 3.9306c0.07675 0.48377 0.16978 0.8838 0.24421 1.1629l4.1004 51.461c0 2.8654 2.6026 5.1912 5.8145 5.1912s5.8145-2.3258 5.8145-5.1912l4.1004-51.447c0.0744-0.28607 0.16746-0.69774 0.23258-1.1629a12.473 12.473 0 0 0 0.0116-3.9376zm-4.2376 7.8868a8.3426 8.3426 0 0 1-3.5375 2.1072c-0.25816 0.07443-0.52098 0.13955-0.7838 0.19072a8.7217 8.7217 0 0 1-1.1978 0.1442c-0.26747 0.01163-0.53726 0.01163-0.80473 0a8.7217 8.7217 0 0 1-1.1978-0.1442c-0.26282-0.05117-0.52563-0.11629-0.78379-0.19072a8.3729 8.3729 0 0 1 0-16.048c0.25816-0.07675 0.52098-0.13955 0.78379-0.19072a8.7217 8.7217 0 0 1 1.1978-0.1442c0.26747-0.01163 0.53726-0.01163 0.80473 0a8.7217 8.7217 0 0 1 1.1978 0.1442c0.26282 0.05117 0.52563 0.11396 0.7838 0.19072a8.3729 8.3729 0 0 1 3.5375 13.955zm-5.9215-54.996a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.6055a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.3729a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.6055a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 72.565a2.791 2.791 0 1 1 2.791-2.791 2.791 2.791 0 0 1-2.791 2.791zm0-8.6055a2.791 2.791 0 1 1 2.791-2.791 2.791 2.791 0 0 1-2.791 2.791zm0-8.3729a2.791 2.791 0 1 1 2.791-2.791 2.791 2.791 0 0 1-2.791 2.791zm0-8.6055a2.791 2.791 0 1 1 2.791-2.791 2.791 2.791 0 0 1-2.791 2.791z";case"single":default:return"m101.6 59.589-4.3167-54.166c0-2.8654-2.6026-5.1912-5.8145-5.1912s-5.8145 2.3258-5.8145 5.1912l-4.3167 54.166a8.3264 8.3264 0 0 0-0.10234 1.2792c0 5.047 4.5818 9.1381 10.234 9.1381s10.234-4.0911 10.234-9.1381a8.3264 8.3264 0 0 0-0.10233-1.2792zm-10.131-48.658a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.6055a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.3729a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm0 8.6055a2.791 2.791 0 1 1-2.791 2.791 2.791 2.791 0 0 1 2.791-2.791zm5.9215 29.412a8.3729 8.3729 0 1 1 0-11.843 8.3729 8.3729 0 0 1 0 11.843z"}}render(){var e;return G`
      <svg
        xmlns="http://www.w3.org/2000/svg"
        width="45mm"
        height="31.63mm"
        version="1.1"
        viewBox="0 0 170.08 119.55"
      >
        <defs>
          <g id="pin">
            <rect x="0" y="-1.91" width="3.72" height="3.71" />
            <path d="m2.026 -1.91h13.532l-13.425 0.51865z" />
            <path d="m2.026 1.80h13.532l-13.425-0.50702z" />
            <rect fill="#ccc" x="0.33" y="-1.23" width="3.04" height="2.46" rx=".15" />
          </g>
        </defs>
        <g stroke-width="2.7" fill="none">
          <path
            stroke="#b44200"
            d="m 83.32,56.6 c0,0 -32.99,0.96 -43.32,0 -6.20,-0.58 -10.60,-6.20 -14.87,-6.31"
          />
          <path stroke="#ff2300" d="m83.326 59.6h-62.971" />
          <path
            stroke="#f47b00"
            d="m 83.32,62.6 c0,0 -32.60,-0.61 -43.33,-0.15 -6.87,0.29 -12.01,6.82 -14.77,6.73"
          />
        </g>
        <rect fill="#666" y="45.5" width="25.71" height="28" rx="1.14" />
        <use xlink:href="#pin" x="4.7" y="50.06" />
        <use xlink:href="#pin" x="4.7" y="59.66" />
        <use xlink:href="#pin" x="4.7" y="69.26" />
        <path
          fill="#4d4d4d"
          d="m163.92 66.867a7.09 7.09 0 1 1 5.8145-11.136 0.18 0.18 0 0 0 0.33-0.10234v-14.346h-17.664v36.98h17.676v-14.346a0.18 0.18 0 0 0-0.333-0.107 7.08 7.08 0 0 1-5.83 3.06z"
        />
        <path
          fill="#4d4d4d"
          d="m55.068 66.75a7.09 7.09 0 1 0-5.8261-11.136 0.18 0.18 0 0 1-0.33-0.10234v-14.346h17.676v36.98h-17.676v-14.346a0.18 0.18 0 0 1 0.333-0.107 7.08 7.08 0 0 0 5.83 3.06z"
        />
        <rect fill="#666" x="64.255" y="37.911" width="90.241" height="43.725" rx="5.3331" />
        <path fill="gray" d="m110.07 50.005h-14.42v19.537h14.42a9.7684 9.7684 0 0 0 0-19.537z" />
        <circle fill="#999" cx="91.467" cy="59.773" r="18.606" />
        <path
          fill=${this.hornColor}
          transform="translate(91.467 59.773) rotate(${(e=this.angle)!==null&&e!==void 0?e:0}) translate(-91.467 -59.773)"
          d="${this.hornPath()}"
        />
        <circle fill="gray" cx="91.467" cy="59.773" r="8.3729" />
        <circle fill="#ccc" cx="91.467" cy="59.773" r="6.2494" />
        <path
          fill="#666"
          d="m94.911 62.543-2.3839-2.4165a0.42562 0.42562 0 0 1 0-0.60471l2.4281-2.3863a0.64657 0.64657 0 0 0 0.06512-0.8652 0.62797 0.62797 0 0 0-0.93032-0.05117l-2.4351 2.4049a0.4326 0.4326 0 0 1-0.60703 0l-2.3863-2.4165a0.6489 0.6489 0 0 0-0.8652-0.06512 0.63262 0.63262 0 0 0-0.05117 0.93032l2.4049 2.4328a0.42795 0.42795 0 0 1 0 0.60703l-2.4142 2.3863a0.65122 0.65122 0 0 0-0.06745 0.8652 0.63029 0.63029 0 0 0 0.93032 0.05117l2.4351-2.4049a0.42562 0.42562 0 0 1 0.60471 0l2.3863 2.4398a0.63262 0.63262 0 0 0 0.93032-0.04186 0.64657 0.64657 0 0 0-0.04419-0.8652z"
        />
      </svg>
    `}};zi([y()],p1.prototype,"angle",void 0);zi([y()],p1.prototype,"horn",void 0);zi([y()],p1.prototype,"hornColor",void 0);p1=zi([U("wokwi-servo")],p1);var f3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let x2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"VCC",x:10,y:114.9,signals:[{type:"power",signal:"VCC"}],number:1},{name:"SDA",x:22.4,y:114.9,signals:[],number:2},{name:"NC",x:35.3,y:114.9,signals:[],number:3},{name:"GND",x:48,y:114.9,signals:[{type:"power",signal:"GND"}],number:4}]}render(){return G`
      <svg
        width="15.1mm"
        height="30.885mm"
        version="1.1"
        viewBox="0 0 15.1 30.885"
        xmlns="http://www.w3.org/2000/svg"
      >
        <g fill="#ccc" stroke-linecap="round" stroke-width=".21">
          <rect x="2.27" y="23.885" width=".75" height="7" rx=".2" />
          <rect x="5.55" y="23.885" width=".75" height="7" rx=".2" />
          <rect x="8.96" y="23.885" width=".75" height="7" rx=".2" />
          <rect x="12.32" y="23.885" width=".75" height="7" rx=".2" />
        </g>
        <path
          d="M15.05 23.995V5.033m0 0c0-.107-1.069-4.962-2.662-4.96L2.803.09C1.193.09.05 4.926.05 5.033v18.962c0 .107.086.192.192.192h14.616a.192.192 0 00.192-.192M7.615.948h.004c1.08 0 1.956.847 1.956 1.892s-.876 1.892-1.956 1.892-1.956-.847-1.956-1.892c0-1.044.873-1.89 1.952-1.892zM4.967 8.66H5.9a.21.21 0 01.211.21v.935a.21.21 0 01-.21.21h-.934a.21.21 0 01-.212-.21V8.87a.21.21 0 01.212-.211zm2.168 0h.934a.21.21 0 01.21.21v.935a.21.21 0 01-.21.21h-.934a.21.21 0 01-.21-.21V8.87a.21.21 0 01.21-.211zm2.152 0h.935a.21.21 0 01.21.21v.935a.21.21 0 01-.21.21h-.935a.21.21 0 01-.21-.21V8.87a.21.21 0 01.21-.211zm5.757 0v1.356m0 0h-3.217a.553.553 0 01-.554-.554v-.249a.55.55 0 01.554-.553h3.217M.05 8.66h3.282c.307 0 .554.247.554.553v.25a.552.552 0 01-.554.553H.05m0 1.054h3.282c.307 0 .554.247.554.554v.249a.552.552 0 01-.554.554H.05m4.917-1.357H5.9a.21.21 0 01.211.211v.934a.21.21 0 01-.21.211h-.934a.21.21 0 01-.212-.21v-.935a.21.21 0 01.212-.21zm2.168 0h.934a.21.21 0 01.211.211v.934a.21.21 0 01-.211.211h-.934a.21.21 0 01-.21-.21v-.935a.21.21 0 01.21-.21zm2.153 0h.934a.21.21 0 01.21.211v.934a.21.21 0 01-.21.211h-.934a.21.21 0 01-.211-.21v-.935a.21.21 0 01.21-.21zm2.539 0h3.217v1.356h-3.217a.552.552 0 01-.554-.553v-.25c0-.306.247-.553.554-.553zM.05 13.547h3.282c.307 0 .553.247.553.554v.249a.552.552 0 01-.553.553H.05m4.916-1.356H5.9a.21.21 0 01.211.211v.934a.21.21 0 01-.21.211h-.935a.21.21 0 01-.21-.21v-.935a.21.21 0 01.21-.21zm2.169 0h.933a.21.21 0 01.212.211v.934a.21.21 0 01-.212.211h-.933a.21.21 0 01-.211-.21v-.935a.21.21 0 01.21-.21zm2.152 0h.934a.21.21 0 01.211.211v.934a.21.21 0 01-.21.211h-.935a.21.21 0 01-.21-.21v-.935a.21.21 0 01.21-.21zm5.757 1.356h-3.217a.552.552 0 01-.554-.553v-.25c0-.306.247-.553.554-.553h3.217m0 3.791h-3.218a.553.553 0 01-.553-.554v-.249c0-.306.247-.553.553-.553h3.218m-14.994 0h3.282c.307 0 .553.247.553.553v.25a.552.552 0 01-.553.553H.05m4.916-1.356H5.9a.21.21 0 01.211.211v.934a.21.21 0 01-.21.21h-.935a.21.21 0 01-.21-.21v-.934a.21.21 0 01.21-.21zm2.169 0h.934a.21.21 0 01.21.211v.934a.21.21 0 01-.21.21h-.934a.21.21 0 01-.211-.21v-.934a.21.21 0 01.211-.21zm2.152 0h.934a.21.21 0 01.211.211v.934a.21.21 0 01-.21.21h-.935a.21.21 0 01-.21-.21v-.934a.21.21 0 01.21-.21zM.05 18.362h3.282c.307 0 .553.247.553.554v.25a.552.552 0 01-.553.552H.05m4.916-1.355H5.9a.21.21 0 01.211.21v.934a.21.21 0 01-.21.211h-.935a.21.21 0 01-.21-.21v-.934a.21.21 0 01.21-.211zm2.169 0h.933a.21.21 0 01.212.21v.934a.21.21 0 01-.212.211h-.933a.21.21 0 01-.211-.21v-.934a.21.21 0 01.21-.211zm2.152 0h.934a.21.21 0 01.211.21v.934a.21.21 0 01-.21.211h-.935a.21.21 0 01-.21-.21v-.934a.21.21 0 01.21-.211zm5.757 1.355h-3.218a.552.552 0 01-.553-.553v-.25c0-.306.247-.552.553-.552h3.218M10.49 5.056V7.31a.192.192 0 01-.193.193h-.85a.192.192 0 01-.193-.193V5.056H8.23v2.286a.192.192 0 01-.192.192h-.851a.192.192 0 01-.193-.192V5.056H5.94v2.286a.192.192 0 01-.193.192h-.85a.192.192 0 01-.193-.192V5.056C.033 5.025.05 5.033.05 5.033m15 0l-4.56.023v0"
          fill="#f2f2f2"
          stroke="#000"
          stroke-linecap="round"
          stroke-width=".1"
        />
        <text
          x="3.7415893"
          y="22.863354"
          fill="#000000"
          font-family="sans-serif"
          font-size="2.2px"
          stroke-width=".05"
          style="line-height:1.25"
        >
          DHT22
        </text>
      </svg>
    `}};x2=f3([U("wokwi-dht22")],x2);var nt=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let F0=class extends L{constructor(){super(...arguments);this.led13=!1,this.ledRX=!1,this.ledTX=!1,this.ledPower=!1,this.resetPressed=!1,this.pinInfo=[{name:"SCL",x:90,y:9,signals:[ve("SCL")]},{name:"SDA",x:100,y:9,signals:[ve("SDA")]},{name:"AREF",x:109,y:9,signals:[]},{name:"GND.1",x:119,y:9,signals:[{type:"power",signal:"GND"}]},{name:"13",x:129,y:9,signals:[{type:"pwm"}]},{name:"12",x:138,y:9,signals:[{type:"pwm"}]},{name:"11",x:148,y:9,signals:[{type:"pwm"}]},{name:"10",x:157.5,y:9,signals:[{type:"pwm"}]},{name:"9",x:167.5,y:9,signals:[{type:"pwm"}]},{name:"8",x:177,y:9,signals:[{type:"pwm"}]},{name:"7",x:190,y:9,signals:[{type:"pwm"}]},{name:"6",x:200,y:9,signals:[{type:"pwm"}]},{name:"5",x:209.5,y:9,signals:[{type:"pwm"}]},{name:"4",x:219,y:9,signals:[{type:"pwm"}]},{name:"3",x:228.5,y:9,signals:[{type:"pwm"}]},{name:"2",x:238,y:9,signals:[{type:"pwm"}]},{name:"1",x:247.5,y:9,signals:[ze("TX")]},{name:"0",x:257.5,y:9,signals:[ze("RX")]},{name:"14",x:270.5,y:9,signals:[ze("TX",3)]},{name:"15",x:280,y:9,signals:[ze("RX",3)]},{name:"16",x:289.5,y:9,signals:[ze("TX",2)]},{name:"17",x:299,y:9,signals:[ze("RX",2)]},{name:"18",x:308.5,y:9,signals:[ze("TX",1)]},{name:"19",x:318.5,y:9,signals:[ze("RX",1)]},{name:"20",x:328,y:9,signals:[ve("SDA")]},{name:"21",x:337.5,y:9,signals:[ve("SCL")]},{name:"5V.1",x:361,y:8,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"5V.2",x:371,y:8,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"22",x:361,y:17.5,signals:[]},{name:"23",x:371,y:17.5,signals:[]},{name:"24",x:361,y:27.25,signals:[]},{name:"25",x:371,y:27.25,signals:[]},{name:"26",x:361,y:36.75,signals:[]},{name:"27",x:371,y:36.75,signals:[]},{name:"28",x:361,y:46.25,signals:[]},{name:"29",x:371,y:46.25,signals:[]},{name:"30",x:361,y:56,signals:[]},{name:"31",x:371,y:56,signals:[]},{name:"32",x:361,y:65.5,signals:[]},{name:"33",x:371,y:65.5,signals:[]},{name:"34",x:361,y:75,signals:[]},{name:"35",x:371,y:75,signals:[]},{name:"36",x:361,y:84.5,signals:[]},{name:"37",x:371,y:84.5,signals:[]},{name:"38",x:361,y:94.25,signals:[]},{name:"39",x:371,y:94.25,signals:[]},{name:"40",x:361,y:103.75,signals:[]},{name:"41",x:371,y:103.75,signals:[]},{name:"42",x:361,y:113.5,signals:[]},{name:"43",x:371,y:113.5,signals:[]},{name:"44",x:361,y:123,signals:[{type:"pwm"}]},{name:"45",x:371,y:123,signals:[{type:"pwm"}]},{name:"46",x:361,y:132.75,signals:[{type:"pwm"}]},{name:"47",x:371,y:132.75,signals:[]},{name:"48",x:361,y:142.25,signals:[]},{name:"49",x:371,y:142.25,signals:[]},{name:"50",x:361,y:152,signals:[te("MISO")]},{name:"51",x:371,y:152,signals:[te("MOSI")]},{name:"52",x:361,y:161.5,signals:[te("SCK")]},{name:"53",x:371,y:161.5,signals:[te("SS")]},{name:"GND.4",x:361,y:171.25,signals:[{type:"power",signal:"GND"}]},{name:"GND.5",x:371,y:171.25,signals:[{type:"power",signal:"GND"}]},{name:"IOREF",x:136,y:184.5,signals:[]},{name:"RESET",x:145.5,y:184.5,signals:[]},{name:"3.3V",x:155,y:184.5,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"5V",x:164.5,y:184.5,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"GND.2",x:174.25,y:184.5,signals:[{type:"power",signal:"GND"}]},{name:"GND.3",x:183.75,y:184.5,signals:[{type:"power",signal:"GND"}]},{name:"VIN",x:193.5,y:184.5,signals:[{type:"power",signal:"VCC"}]},{name:"A0",x:208.5,y:184.5,signals:[H(0)]},{name:"A1",x:218,y:184.5,signals:[H(1)]},{name:"A2",x:227.5,y:184.5,signals:[H(2)]},{name:"A3",x:237.25,y:184.5,signals:[H(3)]},{name:"A4",x:246.75,y:184.5,signals:[H(4)]},{name:"A5",x:256.25,y:184.5,signals:[H(5)]},{name:"A6",x:266,y:184.5,signals:[H(6)]},{name:"A7",x:275.5,y:184.5,signals:[H(7)]},{name:"A8",x:290.25,y:184.5,signals:[H(8)]},{name:"A9",x:300,y:184.5,signals:[H(9)]},{name:"A10",x:309.5,y:184.5,signals:[H(10)]},{name:"A11",x:319.25,y:184.5,signals:[H(11)]},{name:"A12",x:328.75,y:184.5,signals:[H(12)]},{name:"A13",x:338.5,y:184.5,signals:[H(13)]},{name:"A14",x:348,y:184.5,signals:[H(14)]},{name:"A15",x:357.75,y:184.5,signals:[H(15)]}]}static get styles(){return ke`
      text {
        font-size: 2px;
        font-family: monospace;
      }
      circle[tabindex]:hover,
      circle[tabindex]:focus {
        stroke: white;
        outline: none;
      }
    `}render(){const{ledPower:e,led13:i,ledRX:s,ledTX:n}=this;return G`
      <svg
        width="102.66mm"
        height="50.80mm"
        version="1.1"
        viewBox="-4 0 102.66 50.80"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <g id="led-body" fill="#eee">
            <rect x="0" y="0" height="1.2" width="2.6" fill="#c6c6c6" />
            <rect x="0.6" y="-0.1" width="1.35" height="1.4" stroke="#aaa" stroke-width="0.05" />
          </g>
        </defs>

        <filter id="ledFilter" x="-0.8" y="-0.8" height="2.2" width="2.8">
          <feGaussianBlur stdDeviation="0.5" />
        </filter>

        ${vi}

        <pattern id="pin-male" width="2.54" height="4.80" patternUnits="userSpaceOnUse">
          <rect ry="0.3" rx="0.3" width="2.12" height="4.80" fill="#565656" />
          <ellipse cx="1" cy="1.13" rx="0.5" ry="0.5" fill="#aaa"></ellipse>
          <ellipse cx="1" cy="3.67" rx="0.5" ry="0.5" fill="#aaa"></ellipse>
        </pattern>

        <!-- PCB -->
        <path
          d="M2.105.075v50.653h94.068v-1.206l2.412-2.412V14.548l-2.412-2.413V2.487L93.785.075zm14.443.907a1.505 1.505 0 01.03 0 1.505 1.505 0 011.504 1.505 1.505 1.505 0 01-1.504 1.506 1.505 1.505 0 01-1.506-1.506A1.505 1.505 0 0116.548.982zm71.154 0a1.505 1.505 0 01.03 0 1.505 1.505 0 011.505 1.505 1.505 1.505 0 01-1.505 1.506 1.505 1.505 0 01-1.506-1.506A1.505 1.505 0 0187.702.982zM64.818 15.454a1.505 1.505 0 011.504 1.506 1.505 1.505 0 01-1.504 1.505 1.505 1.505 0 01-1.506-1.505 1.505 1.505 0 011.506-1.506zm0 26.532a1.505 1.505 0 011.504 1.506 1.505 1.505 0 01-1.504 1.505 1.505 1.505 0 01-1.506-1.505 1.505 1.505 0 011.506-1.506zm-49.476 4.825a1.505 1.505 0 01.03 0 1.505 1.505 0 011.505 1.504 1.505 1.505 0 01-1.506 1.506 1.505 1.505 0 01-1.505-1.506 1.505 1.505 0 011.476-1.504zm78.39 0a1.505 1.505 0 01.03 0 1.505 1.505 0 011.504 1.504 1.505 1.505 0 01-1.504 1.506 1.505 1.505 0 01-1.506-1.506 1.505 1.505 0 011.476-1.504z"
          fill="#2b6b99"
        />

        <!-- reset button -->
        <rect
          transform="rotate(269.81)"
          x="-28.046"
          y="68.977"
          width="6.2151"
          height="6.0268"
          fill="#9b9b9b"
        />
        <g fill="#e6e6e6">
          <rect transform="rotate(269.81)" x="-29.725" y="69.518" width="1.695" height=".84994" />
          <rect transform="rotate(269.81)" x="-29.741" y="71.4" width="1.695" height=".84994" />
          <rect transform="rotate(269.81)" x="-29.764" y="73.425" width="1.695" height=".84994" />
          <rect transform="rotate(269.81)" x="-21.831" y="73.59" width="1.695" height=".84994" />
          <rect transform="rotate(269.81)" x="-21.854" y="69.517" width="1.695" height=".84994" />
        </g>
        <circle
          id="reset-button"
          transform="rotate(269.81)"
          cx="-24.9"
          cy="72.092"
          r="1.5405"
          fill="#960000"
          stroke="#777"
          stroke-width="0.15"
          tabindex="0"
          @mousedown=${()=>this.down()}
          @touchstart=${()=>this.down()}
          @mouseup=${()=>this.up()}
          @mouseleave=${()=>this.leave()}
          @touchend=${()=>this.leave()}
          @keydown=${r=>Te.includes(r.key)&&this.down()}
          @keyup=${r=>Te.includes(r.key)&&this.up()}
        />

        <!-- USB Connector -->
        <g style="fill:#b3b2b2;stroke:#b3b2b2;stroke-width:0.010">
          <ellipse cx="3.84" cy="9.56" rx="1.12" ry="1.03" />
          <ellipse cx="3.84" cy="21.04" rx="1.12" ry="1.03" />
          <g fill="#000">
            <rect width="11" height="11.93" x="-0.05" y="9.72" rx="0.2" ry="0.2" opacity="0.24" />
          </g>
          <rect x="-4" y="9.37" height="11.85" width="14.46" />
          <rect x="-4" y="9.61" height="11.37" width="14.05" fill="#706f6f" />
          <rect x="-4" y="9.71" height="11.17" width="13.95" fill="#9d9d9c" />
        </g>

        <!-- Power jack -->
        <g stroke-width=".254" fill="black" transform="translate(0 -4)">
          <path
            d="m-2.58 48.53v2.289c0 0.279 0.228 0.508 0.508 0.508h1.722c0.279 0 0.508-0.228 0.508-0.508v-2.289z"
            fill="#252728"
            opacity=".3"
          />
          <path
            d="m11.334 42.946c0-0.558-0.509-1.016-1.132-1.016h-10.043v9.652h10.043c0.622 0 1.132-0.457 1.132-1.016z"
            opacity=".3"
          />
          <path
            d="m-2.072 40.914c-0.279 0-0.507 0.204-0.507 0.454v8.435c0 0.279 0.228 0.507 0.507 0.507h1.722c0.279 0 0.507-0.228 0.507-0.507v-8.435c0-0.249-0.228-0.454-0.507-0.454z"
          />
          <path
            d="m-2.58 48.784v1.019c0 0.279 0.228 0.508 0.508 0.508h1.722c0.279 0 0.508-0.228 0.508-0.508v-1.019z"
            opacity=".3"
          />
          <path
            d="m11.334 43.327c0.139 0 0.254 0.114 0.254 0.254v4.064c0 0.139-0.114 0.254-0.254 0.254"
          />
          <path
            d="m11.334 42.438c0-0.558-0.457-1.016-1.016-1.016h-10.16v8.382h10.16c0.558 0 1.016-0.457 1.016-1.016z"
          />
          <path
            d="m10.064 49.804h-9.906v-8.382h1.880c-1.107 0-1.363 1.825-1.363 3.826 0 1.765 1.147 3.496 3.014 3.496h6.374z"
            opacity=".3"
          />
          <rect x="10.064" y="41.422" width=".254" height="8.382" fill="#ffffff" opacity=".2" />
          <path
            d="m10.318 48.744v1.059c0.558 0 1.016-0.457 1.016-1.016v-0.364c0 0.313-1.016 0.320-1.016 0.320z"
            opacity=".3"
          />
        </g>

        <!-- Pin Headers -->
        <g transform="translate(18.4 1.07)">
          <rect width="${.38+2.54*10}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(44.81 1.07)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(66 1.07)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(27.93 47.5)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(49.63 47.5)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(71.34 47.5)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)"></rect>
        </g>
        <g transform="translate(90.1 0.8)">
          <rect width="${.38+2.54*2}" height="${2.54*18}" fill="url(#pins-female)"></rect>
        </g>

        <!-- MCU -->
        <rect x="43" y="17.55" fill="#000" width="13.5" height="13.5" rx="0.5" />

        <!-- Programming Headers -->
        <g transform="translate(61.5 21.09)">
          <rect width="4.80" height="7" fill="url(#pin-male)" />
        </g>

        <!-- LEDs -->
        <g transform="translate(72.20 15.58)">
          <use xlink:href="#led-body" />
          ${e&&V`<circle cx="1.3" cy="0.55" r="1.3" fill="#80ff80" filter="url(#ledFilter)" />`}
        </g>

        <text fill="#fff">
          <tspan x="68" y="16.75">PWR</tspan>
        </text>

        <g transform="translate(26 13.86)">
          <use xlink:href="#led-body" />
          ${i&&V`<circle cx="1.3" cy="0.55" r="1.3" fill="#ff8080" filter="url(#ledFilter)" />`}
        </g>

        <g transform="translate(26 18.52)">
          <use xlink:href="#led-body" />
          ${n&&V`<circle cx="0.975" cy="0.55" r="1.3" fill="yellow" filter="url(#ledFilter)" />`}
        </g>

        <g transform="translate(26 20.75)">
          <use xlink:href="#led-body" />
          ${s&&V`<circle cx="0.975" cy="0.55" r="1.3" fill="yellow" filter="url(#ledFilter)" />`}
        </g>

        <text fill="#fff">
          <tspan x="29.4" y="15">L</tspan>
          <tspan x="29.4" y="19.8">TX</tspan>
          <tspan x="29.4" y="22">RX</tspan>
          <tspan x="26.5" y="20">&nbsp;</tspan>
        </text>

        <!-- Pin Labels -->
        <rect x="28.27" y="7.6" width="31.5" height="0.16" fill="#fff"></rect>
        <text fill="#fff" style="font-weight: 900">
          <tspan x="40.84" y="9.8">PWM</tspan>
        </text>

        <rect x="60.5" y="11.8" width="25.4" height="0.16" fill="#fff"></rect>
        <text fill="#fff" style="font-weight: 900">
          <tspan x="65" y="14.2">COMMUNICATION</tspan>
        </text>

        <text
          transform="translate(22.6 3.4) rotate(270 0 0)"
          fill="#fff"
          style="font-size: 2px; text-anchor: end; font-family: monospace"
        >
          <tspan x="0" dy="2.54">AREF</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">13</tspan>
          <tspan x="0" dy="2.54">12</tspan>
          <tspan x="0" dy="2.54">11</tspan>
          <tspan x="0" dy="2.54">10</tspan>
          <tspan x="0" dy="2.54">9</tspan>
          <tspan x="0" dy="2.54">8</tspan>
          <tspan x="0" dy="4.08">7</tspan>
          <tspan x="0" dy="2.54">6</tspan>
          <tspan x="0" dy="2.54">5</tspan>
          <tspan x="0" dy="2.54">4</tspan>
          <tspan x="0" dy="2.54">3</tspan>
          <tspan x="0" dy="2.54">2</tspan>
          <tspan x="0" dy="2.54">TX→ 1</tspan>
          <tspan x="0" dy="2.54">RX← 0</tspan>
          <tspan x="0" dy="3.34">TX3 14</tspan>
          <tspan x="0" dy="2.54">RX3 15</tspan>
          <tspan x="0" dy="2.54">TX2 16</tspan>
          <tspan x="0" dy="2.54">RX2 17</tspan>
          <tspan x="0" dy="2.54">TX1 18</tspan>
          <tspan x="0" dy="2.54">RX1 19</tspan>
          <tspan x="0" dy="2.54">SDA 20</tspan>
          <tspan x="0" dy="2.54">SCL 21</tspan>
          <tspan x="0" dy="2.54">&nbsp;</tspan>
        </text>

        <rect x="36" y="41.46" width="12.44" height="0.16" fill="#fff"></rect>
        <rect x="50" y="41.46" width="39" height="0.16" fill="#fff"></rect>
        <text fill="#fff" style="font-weight: 900">
          <tspan x="39" y="40.96">POWER</tspan>
          <tspan x="65" y="40.96">ANALOG IN</tspan>
        </text>
        <text transform="translate(30.19 47) rotate(270 0 0)" fill="#fff" style="font-weight: 700">
          <tspan x="0" dy="2.54">IOREF</tspan>
          <tspan x="0" dy="2.54">RESET</tspan>
          <tspan x="0" dy="2.54">3.3V</tspan>
          <tspan x="0" dy="2.54">5V</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">GND</tspan>
          <tspan x="0" dy="2.54">Vin</tspan>
          <tspan x="0" dy="3.74">A0</tspan>
          <tspan x="0" dy="2.54">A1</tspan>
          <tspan x="0" dy="2.54">A2</tspan>
          <tspan x="0" dy="2.54">A3</tspan>
          <tspan x="0" dy="2.54">A4</tspan>
          <tspan x="0" dy="2.54">A5</tspan>
          <tspan x="0" dy="2.54">A6</tspan>
          <tspan x="0" dy="2.54">A7</tspan>
          <tspan x="0" dy="3.74">A8</tspan>
          <tspan x="0" dy="2.54">A9</tspan>
          <tspan x="0" dy="2.54">A10</tspan>
          <tspan x="0" dy="2.54">A11</tspan>
          <tspan x="0" dy="2.54">A12</tspan>
          <tspan x="0" dy="2.54">A13</tspan>
          <tspan x="0" dy="1.84">A14</tspan>
          <tspan x="0" dy="1.84">A15</tspan>
          <tspan x="0" dy="2.54">&nbsp;</tspan>
        </text>

        <!-- Logo -->
        <text x="51.85" y="36" style="font-size:4px;font-weight:bold;line-height:1.25;fill:#fff">
          Arduino MEGA
        </text>
      </svg>
    `}down(){this.resetPressed||(this.resetPressed=!0,this.resetButton.style.stroke="#333",this.dispatchEvent(new CustomEvent("button-press",{detail:"reset"})))}up(){!this.resetPressed||(this.resetPressed=!1,this.resetButton.style.stroke="",this.dispatchEvent(new CustomEvent("button-release",{detail:"reset"})))}leave(){this.resetButton.blur(),this.up()}};nt([y()],F0.prototype,"led13",void 0);nt([y()],F0.prototype,"ledRX",void 0);nt([y()],F0.prototype,"ledTX",void 0);nt([y()],F0.prototype,"ledPower",void 0);nt([y()],F0.prototype,"resetPressed",void 0);nt([v1("#reset-button")],F0.prototype,"resetButton",void 0);F0=nt([U("wokwi-arduino-mega")],F0);var rt=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let I0=class extends L{constructor(){super(...arguments);this.led13=!1,this.ledRX=!1,this.ledTX=!1,this.ledPower=!1,this.resetPressed=!1,this.pinInfo=[{name:"12",x:19.7,y:4.8,signals:[te("MISO")]},{name:"11",x:29.3,y:4.8,signals:[te("MOSI"),{type:"pwm"}]},{name:"10",x:38.9,y:4.8,signals:[te("SS"),{type:"pwm"}]},{name:"9",x:48.5,y:4.8,signals:[{type:"pwm"}]},{name:"8",x:58.1,y:4.8,signals:[]},{name:"7",x:67.7,y:4.8,signals:[]},{name:"6",x:77.3,y:4.8,signals:[{type:"pwm"}]},{name:"5",x:86.9,y:4.8,signals:[{type:"pwm"}]},{name:"4",x:96.5,y:4.8,signals:[]},{name:"3",x:106.1,y:4.8,signals:[{type:"pwm"}]},{name:"2",x:115.7,y:4.8,signals:[]},{name:"GND.2",x:125.3,y:4.8,signals:[{type:"power",signal:"GND"}]},{name:"RESET.2",x:134.9,y:4.8,signals:[]},{name:"1",x:144.5,y:4.8,signals:[ze("TX")]},{name:"0",x:154.1,y:4.8,signals:[ze("RX")]},{name:"13",x:19.7,y:62.4,signals:[te("SCK")]},{name:"3.3V",x:29.3,y:62.4,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"AREF",x:38.9,y:62.4,signals:[]},{name:"A0",x:48.5,y:62.4,signals:[H(0)]},{name:"A1",x:58.1,y:62.4,signals:[H(1)]},{name:"A2",x:67.7,y:62.4,signals:[H(2)]},{name:"A3",x:77.3,y:62.4,signals:[H(3)]},{name:"A4",x:86.9,y:62.4,signals:[H(4),ve("SDA")]},{name:"A5",x:96.5,y:62.4,signals:[H(5),ve("SCL")]},{name:"A6",x:106.1,y:62.4,signals:[H(6)]},{name:"A7",x:115.7,y:62.4,signals:[H(7)]},{name:"5V",x:125.3,y:62.4,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"RESET",x:134.9,y:62.4,signals:[]},{name:"GND.1",x:144.5,y:62.4,signals:[{type:"power",signal:"GND"}]},{name:"VIN",x:154.1,y:62.4,signals:[{type:"power",signal:"VCC"}]},{name:"12.2",x:163.7,y:43.2,signals:[te("MISO")]},{name:"5V.2",x:154.1,y:43.2,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"13.2",x:163.7,y:33.6,signals:[te("SCK")]},{name:"11.2",x:154.1,y:33.6,signals:[te("MOSI"),{type:"pwm"}]},{name:"RESET.3",x:163.7,y:24,signals:[]},{name:"GND.3",x:154.1,y:24,signals:[{type:"power",signal:"GND"}]}]}static get styles(){return ke`
      text {
        font-size: 1px;
        font-family: monospace;
        user-select: none;
      }

      circle[tabindex]:hover,
      circle[tabindex]:focus {
        stroke: white;
        outline: none;
      }
    `}render(){const{ledPower:e,led13:i,ledRX:s,ledTX:n}=this;return G`
      <svg
        width="44.9mm"
        height="17.8mm"
        viewBox="-1.4 0 44.9 17.8"
        font-weight="bold"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <filter id="solderPlate" style="color-interpolation-filters:sRGB;">
            <feTurbulence result="r0" type="fractalNoise" baseFrequency="1" numOctaves="1" />
            <feComposite
              result="r1"
              in="r0"
              in2="SourceGraphic"
              operator="arithmetic"
              k1=".6"
              k2=".6"
              k3="1.2"
              k4=".25"
            />
            <feBlend result="r2" in="r1" in2="SourceGraphic" mode="luminosity" />
            <feComposite result="r3" in="r2" in2="SourceGraphic" operator="in" />
          </filter>
          <pattern id="pins-tqfp-0.5mm" width="1" height=".5" patternUnits="userSpaceOnUse">
            <rect fill="#333" width="1" height=".2" y=".17" />
          </pattern>
          <pattern id="pins-pth-0.75" width="2.54" height="2.54" patternUnits="userSpaceOnUse">
            <circle r=".75" cx="1.27" cy="1.27" fill="#333" filter="url(#solderPlate)" />
            <g stroke="#333" stroke-width=".05" paint-order="stroke fill">
              <circle r=".375" cx="1.27" cy="1.27" fill="#eee" />
            </g>
          </pattern>
          <pattern id="pins-pth-1.00" width="2.54" height="2.54" patternUnits="userSpaceOnUse">
            <circle r=".75" cx="1.27" cy="1.27" fill="#333" filter="url(#solderPlate)" />
            <g stroke="#333" stroke-width=".05" paint-order="stroke fill">
              <circle r=".5" cx="1.27" cy="1.27" fill="#eee" />
            </g>
          </pattern>
          <g id="led-body" fill="#eee">
            <rect x="0" y="0" height="1.2" width="2.6" fill="#333" filter="url(#solderPlate)" />
            <rect x=".6" y="-0.1" width="1.35" height="1.4" stroke="#aaa" stroke-width=".05" />
          </g>
          <filter id="ledFilter" x="-0.8" y="-0.8" height="2.2" width="2.8">
            <feGaussianBlur stdDeviation=".5" />
          </filter>
        </defs>

        <!-- PCB -->
        <g id="pcb" fill="#fff">
          <rect width="43.5" height="17.8" x="0" y="0" fill="#1b7e84" />
          <circle cx="1" cy="1" r=".889" />
          <circle cx="42.42" cy="1" r=".889" />
          <circle cx="42.42" cy="16.6" r=".889" />
          <circle cx="1" cy="16.6" r=".889" />
        </g>

        <!-- silkscreen -->
        <g id="silkscreen" fill="#eee" text-anchor="middle">
          <rect x="30.48" y="0" width="2.54" height="3.2" />
          <rect x="35.56" y="14.6" width="2.54" height="3.2" />
          <g fill="#1b7e84" transform="translate(1.27 1.27)">
            <circle r=".85" cx="30.48" />
            <circle r=".85" cx="35.56" cy="15.24" />
          </g>
          <g transform="translate(1.27 3)">
            <text x="2.54">D12</text>
            <text x="5.08">D11</text>
            <text x="7.62">D10</text>
            <text x="10.16">D9</text>
            <text x="12.7">D8</text>
            <text x="15.24">D7</text>
            <text x="17.78">D6</text>
            <text x="20.32">D5</text>
            <text x="22.86">D4</text>
            <text x="25.4">D3</text>
            <text x="27.94">D2</text>
            <text x="30.48" fill="#1b7e84">GND</text>
            <text x="33.02">RST</text>
            <text x="35.56" y=".65" font-size="200%">↓</text>
            <text x="35.56" y="1.5">RX0</text>
            <text x="38.1" y=".65" font-size="200%">↑</text>
            <text x="38.1" y="1.5">TX0</text>
          </g>
          <g transform="translate(1.27 15.5)">
            <text x="2.54">D13</text>
            <text x="5.08">3V3</text>
            <text x="7.62">AREF</text>
            <text x="10.16">A0</text>
            <text x="12.7">A1</text>
            <text x="15.24">A2</text>
            <text x="17.78">A3</text>
            <text x="20.32">A4</text>
            <text x="22.86">A5</text>
            <text x="25.4">A6</text>
            <text x="27.94">A7</text>
            <text x="30.48">5V</text>
            <text x="33.02">RST</text>
            <text x="35.56" fill="#1b7e84">GND</text>
            <text x="38.1">VIN</text>
          </g>
          <g transform="rotate(90)">
            <text x="8.7" y="-22.5">RESET</text>
            <text x="5.6" y="-32.2">TX</text>
            <text x="5.6" y="-30.7" font-size="200%">↓</text>
            <text x="7.6" y="-32.2">RX</text>
            <text x="7.6" y="-30.7" font-size="200%">↑</text>
            <text x="9.6" y="-30.7">ON</text>
            <text x="11.6" y="-30.7">L</text>
          </g>
        </g>

        <!-- MCU -->
        <g id="mcu" transform="rotate(45 -2.978 23.39)">
          <g fill="url(#pins-tqfp-0.5mm)" filter="url(#solderPlate)">
            <rect x="-2.65" y="-1.975" width="5.3" height="3.95" />
            <rect x="-2.65" y="-1.975" width="5.3" height="3.95" transform="rotate(90)" />
          </g>
          <rect x="-2.275" y="-2.275" width="4.55" height="4.55" fill="#444" />
          <circle transform="rotate(45)" cx=".012" cy="-2.5" r=".35" fill="#222" />
        </g>

        <!-- pins -->
        <g id="pins" fill="url(#pins-pth-0.75)">
          <g id="pins-pin1" fill="#333" filter="url(#solderPlate)">
            <rect x="${15.5*2.54-.875}" y="${.5*2.54-.875}" width="1.75" height="1.75" />
            <rect x="${15.5*2.54-.875}" y="${6.5*2.54-.875}" width="1.75" height="1.75" />
          </g>
          <rect x="2.54" width="${15*2.54}" height="2.54" />
          <rect x="2.54" y="${6*2.54}" width="${15*2.54}" height="2.54" />
        </g>

        <!-- programming header -->
        <g id="pgm-header" fill="url(#pins-pth-1.00)" stroke="#eee" stroke-width=".1">
          <g id="pgm-pin1" stroke="none" fill="#333" filter="url(#solderPlate)">
            <rect x="${16.5*2.54-.875}" y="${4.5*2.54-.875}" width="1.75" height="1.75" />
          </g>
          <rect x="${15*2.54}" y="${2*2.54}" width="${2*2.54}" height="${3*2.54}" />
        </g>

        <!-- USB mini type B -->
        <g id="usb-mini-b" stroke-width=".1" paint-order="stroke fill">
          <g fill="#333" filter="url(#solderPlate)">
            <rect x=".3" y="3.8" width="1.6" height="9.8" />
            <rect x="5.5" y="3.8" width="1.6" height="9.8" />
            <rect x="7.3" y="7.07" width="1.1" height=".48" />
            <rect x="7.3" y="7.82" width="1.1" height=".48" />
            <rect x="7.3" y="8.58" width="1.1" height=".48" />
            <rect x="7.3" y="9.36" width="1.1" height=".48" />
            <rect x="7.3" y="10.16" width="1.1" height=".48" />
          </g>
          <rect x="-1.4" y="4.8" width="8.9" height="7.8" fill="#999" stroke-width=".26" />
          <rect x="-1.25" y="5" width="8.6" height="7.4" fill="#ccc" stroke="#bbb" />
          <g fill="none" stroke="#333" stroke-width=".26" stroke-linecap="round">
            <path d="m2.6 5.9h-3.3v0.9h3.3m0 3.7h-3.3v0.9h3.3M-0.6 7.6l4.3 0.3v1.5l-4.3 0.3" />
            <path d="m3.3 7.9v1.5" stroke-width="1" stroke-linecap="butt" />
            <path d="m6 6.4v4.5" stroke-width=".35" />
          </g>
        </g>

        <!-- LEDs -->
        <g transform="translate(27.7 5)">
          <use xlink:href="#led-body" />
          ${n&&V`<circle cx="1.3" cy=".55" r="1.3" fill="#ff8080" filter="url(#ledFilter)" />`}
        </g>
        <g transform="translate(27.7 7)">
          <use xlink:href="#led-body" />
          ${s&&V`<circle cx="1.3" cy=".55" r="1.3" fill="#80ff80" filter="url(#ledFilter)" />`}
        </g>
        <g transform="translate(27.7 9)">
          <use xlink:href="#led-body" />
          ${e&&V`<circle cx="1.3" cy=".55" r="1.3" fill="#80ff80" filter="url(#ledFilter)" />`}
        </g>
        <g transform="translate(27.7 11)">
          <use xlink:href="#led-body" />
          ${i&&V`<circle cx="1.3" cy=".55" r="1.3" fill="#ffff80" filter="url(#ledFilter)" />`}
        </g>

        <!-- reset button -->
        <g stroke-width=".1" paint-order="fill stroke">
          <rect x="24.3" y="6.3" width="1" height="4.8" filter="url(#solderPlate)" fill="#333" />
          <rect x="23.54" y="6.8" width="2.54" height="3.8" fill="#ccc" stroke="#888" />
          <circle
            id="reset-button"
            cx="24.8"
            cy="8.7"
            r="1"
            fill="#eeb"
            stroke="#777"
            tabindex="0"
            @mousedown=${()=>this.down()}
            @touchstart=${()=>this.down()}
            @mouseup=${()=>this.up()}
            @mouseleave=${()=>this.leave()}
            @touchend=${()=>this.leave()}
            @keydown=${r=>Te.includes(r.key)&&this.down()}
            @keyup=${r=>Te.includes(r.key)&&this.up()}
          />
        </g>
      </svg>
    `}down(){this.resetPressed||(this.resetPressed=!0,this.resetButton.style.stroke="#333",this.dispatchEvent(new CustomEvent("button-press",{detail:"reset"})))}up(){!this.resetPressed||(this.resetPressed=!1,this.resetButton.style.stroke="",this.dispatchEvent(new CustomEvent("button-release",{detail:"reset"})))}leave(){this.resetButton.blur(),this.up()}};rt([y()],I0.prototype,"led13",void 0);rt([y()],I0.prototype,"ledRX",void 0);rt([y()],I0.prototype,"ledTX",void 0);rt([y()],I0.prototype,"ledPower",void 0);rt([y()],I0.prototype,"resetPressed",void 0);rt([v1("#reset-button")],I0.prototype,"resetButton",void 0);I0=rt([U("wokwi-arduino-nano")],I0);var d3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let v2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",y:15,x:9.5,number:1,signals:[$e()]},{name:"5V",y:25,x:9.5,number:2,signals:[De(5)]},{name:"SDA",y:34.5,x:9.5,number:3,signals:[ve("SDA")]},{name:"SCL",y:44,x:9.5,number:4,signals:[ve("SCL")]},{name:"SQW",y:54,x:9.5,number:5,signals:[]}]}render(){return G`
      <svg
        width="25.8mm"
        height="22.212mm"
        version="1.1"
        viewBox="0 0 25.8 22.212"
        xmlns="http://www.w3.org/2000/svg"
      >
        <path
          d="m2.961 0c-1.6405 0-2.961 1.3207-2.961 2.9611v16.29c0 1.6405 1.3206 2.961 2.961 2.961h19.878c1.6405 0 2.961-1.3206 2.961-2.961v-2.1407c-2.4623-2.4996-2.4864-1.3794-2.4996-5.5588-0.01319-4.1794 0.11192-2.4623 2.4996-5.5961v-2.9945c0-1.6405-1.3206-2.9611-2.961-2.9611zm20.214 1.5792h1.04e-4c3e-3 -1.1e-5 0.0061-1.1e-5 0.0091 0 0.67598-1.6e-5 1.224 0.54798 1.224 1.224 1.5e-5 0.67598-0.54798 1.224-1.224 1.224-0.67598 1.5e-5 -1.224-0.54798-1.224-1.224-3.4e-5 -0.67241 0.54238-1.2189 1.2148-1.224zm-20.564 1.9405c0.29985-2.4e-5 0.54294 0.24306 0.54291 0.54291 2.4e-5 0.29985-0.24306 0.54294-0.54291 0.54291-0.29985 2.4e-5 -0.54294-0.24306-0.54291-0.54291-2.4e-5 -0.29985 0.24306-0.54294 0.54291-0.54291zm-0.02958 2.5853c0.0011-3e-6 0.0021-3e-6 0.0032 0 0.29985-2.4e-5 0.54294 0.24306 0.54291 0.54291-3.2e-5 0.29981-0.2431 0.54283-0.54291 0.54281-0.29977-3.2e-5 -0.54278-0.24304-0.54281-0.54281-2.9e-5 -0.29858 0.24107-0.54114 0.53965-0.54291zm0.02632 2.5062h1.04e-4c0.0011-3e-6 0.0021-3e-6 0.0032 0 0.29985-2.4e-5 0.54294 0.24306 0.54291 0.54291-3.2e-5 0.29981-0.2431 0.54284-0.54291 0.54281-0.29981 2.4e-5 -0.54288-0.243-0.54291-0.54281-2.9e-5 -0.29858 0.24107-0.54114 0.53965-0.54291zm0.02652 2.5853c0.0011-3e-6 0.0021-3e-6 0.0032 0 0.29977 3.2e-5 0.54278 0.24304 0.54281 0.54281 2.4e-5 0.29981-0.243 0.54288-0.54281 0.54291-0.29985 2.4e-5 -0.54294-0.24306-0.54291-0.54291 2.7e-5 -0.29858 0.24117-0.5411 0.53975-0.54281zm-0.02652 2.5325h1.04e-4c0.0011-3e-6 0.0021-3e-6 0.0032 0 0.29985-2.4e-5 0.54294 0.24306 0.54291 0.54291-3.2e-5 0.29981-0.2431 0.54284-0.54291 0.54281-0.29981 2.4e-5 -0.54288-0.243-0.54291-0.54281-2.9e-5 -0.29858 0.24107-0.54114 0.53965-0.54291zm-0.02663 4.4895c3e-3 -1.1e-5 0.0061-1.1e-5 0.0091 0 0.6759 4.2e-5 1.2238 0.54795 1.2238 1.2238 1.5e-5 0.67594-0.54791 1.2239-1.2238 1.224-0.67598 1.5e-5 -1.224-0.54798-1.224-1.224 2.2e-5 -0.67241 0.54248-1.2189 1.2149-1.2238z"
          fill="#015abe"
        />
        <g fill="#ffe680">
          <path
            d="m2.6116 3.0997a0.97608 0.96289 0 0 0-0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606-0.9629 0.97608 0.96289 0 0 0-0.97606-0.9629zm-0.01316 0.40897a0.52761 0.5408 0 0 1 0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761-0.54077 0.52761 0.5408 0 0 1 0.52761-0.54077z"
          />
          <path
            d="m2.5853 5.685a0.97608 0.96289 0 0 0-0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606-0.9629 0.97608 0.96289 0 0 0-0.97606-0.9629zm-0.01316 0.40897a0.52761 0.5408 0 0 1 0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761-0.54077 0.52761 0.5408 0 0 1 0.52761-0.54077z"
          />
          <path
            d="m2.6116 8.1911a0.97608 0.96289 0 0 0-0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606-0.9629 0.97608 0.96289 0 0 0-0.97606-0.9629zm-0.01316 0.40897a0.52761 0.5408 0 0 1 0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761-0.54077 0.52761 0.5408 0 0 1 0.52761-0.54077z"
          />
          <path
            d="m2.638 10.776a0.97608 0.96289 0 0 0-0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606-0.9629 0.97608 0.96289 0 0 0-0.97606-0.9629zm-0.01316 0.40897a0.52761 0.5408 0 0 1 0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761-0.54077 0.52761 0.5408 0 0 1 0.52761-0.54077z"
          />
          <path
            d="m2.6116 13.309a0.97608 0.96289 0 0 0-0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606 0.9629 0.97608 0.96289 0 0 0 0.97606-0.9629 0.97608 0.96289 0 0 0-0.97606-0.9629zm-0.01316 0.40897a0.52761 0.5408 0 0 1 0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761 0.54077 0.52761 0.5408 0 0 1-0.52761-0.54077 0.52761 0.5408 0 0 1 0.52761-0.54077z"
          />
        </g>
        <text transform="rotate(90)" font-size="1.3px" fill="#e6e6e6">
          <tspan x="0.78" y="-3.81">GND</tspan>
          <tspan x="5.75" y="-0.43">5V</tspan>
          <tspan x="7.89" y="-3.81">SDA</tspan>
          <tspan x="10.45" y="-0.49">SCL</tspan>
          <tspan x="13" y="-3.97">SQW</tspan>
        </text>
        <g fill="#999">
          <rect x="6.5174" y="9.8" width=".62" height="1.971" rx=".2" ry=".2" />
          <rect x="6.5174" y="4.29" width=".62" height="1.97" rx=".2" ry=".2" />
          <rect x="7.8138" y="4.26" width=".62" height="1.97" rx=".2" ry=".2" />
          <rect x="7.8138" y="9.77" width=".621" height="1.97" rx=".2" ry=".2" />
          <rect x="9.0674" y="4.26" width=".62" height="1.97" rx=".2" ry=".2" />
          <rect x="10.321" y="4.26" width=".62" height="1.97" rx=".2" ry=".2" />
          <rect x="9.0674" y="9.77" width=".621" height="1.97" rx=".2" ry=".2" />
          <rect x="10.321" y="9.77" width=".621" height="1.97" rx=".2" ry=".2" />
          <rect x="8.8304" y="13" width="1.38" height="1.43" rx=".2" ry=".2" />
          <rect x="5.0064" y="18.56" width="1.38" height="1.43" rx=".2" ry=".2" />
          <rect x="5.0064" y="13.02" width="1.38" height="1.43" rx=".2" ry=".2" />
          <rect x="8.8118" y="18.57" width="1.38" height="1.43" rx=".2" ry=".2" />
        </g>
        <rect x="6.2656" y="6.1049" width="5.1111" height="3.8244" fill="#1a1a1a" />
        <rect x="5.9653" y="12.746" width="3.173" height="7.7357" fill="#1a1a1a" />
        <text fill="#e6e6e6">
          <tspan x="10.5" y="19.8" font-size="2.1px">RTC</tspan>
          <tspan x="10.1" y="21.5" font-size="1.38px">DS1307</tspan>
        </text>
        <path
          d="m23.105 6.4546-0.093544 11.038h-7.6239l-1.4032-2.666-0.14032-7.2965 1.1514-1.1171z"
          fill="#e7d652"
        />
        <path
          transform="scale(.26458)"
          d="m65.771 8.0801c-0.74122-0.056466-0.96289 0.40508-0.96289 0.99805v10.564h-7.7773l-11.018 11.018v26.67l11.191 11.193 7.0625-0.029297v11.404c0.030992 0.86246 0.40014 1.3613 1.3613 1.3613h9.8711c0.79548 0 1.1738-0.34656 1.1738-1.0801v-10.686h7.377s-0.091892-1.0897 0.49805-1.2539c4.3436-1.2091 5.1203-2.5601 5.7949-4.0449 2.0727-4.5618-6.7065-7.6884-6.1094-21.266 0.5971-13.577 7.9939-12.227 6.2988-18.801-0.48161-1.8679-2.1495-3.113-5.6328-3.3926-0.48254-0.038702-0.44922-0.99414-0.44922-0.99414h-7.5781v-10.717c0.00373-0.74652-0.24336-0.94531-1.0918-0.94531h-10.01zm-1.5918 16.668a7.937 7.937 0 0 1 0.14844 0 7.937 7.937 0 0 1 7.9375 7.9355 7.937 7.937 0 0 1-7.9375 7.9375 7.937 7.937 0 0 1-7.9355-7.9375 7.937 7.937 0 0 1 7.7871-7.9355zm0 24.707a7.937 7.937 0 0 1 0.14844 0 7.937 7.937 0 0 1 7.9375 7.9355 7.937 7.937 0 0 1-7.9375 7.9375 7.937 7.937 0 0 1-7.9355-7.9375 7.937 7.937 0 0 1 7.7871-7.9355z"
          fill="#e6e6e6"
        />
        <path
          d="m2.5877 17.819a1.6229 1.6229 0 0 0-1.6198 1.6229 1.6229 1.6229 0 0 0 1.6228 1.6228 1.6229 1.6229 0 0 0 1.6229-1.6228 1.6229 1.6229 0 0 0-1.6229-1.6229 1.6229 1.6229 0 0 0-0.0031 0zm0.0031 0.43845a1.1471 1.1471 0 0 1 1.1471 1.1471 1.1471 1.1471 0 0 1-1.1471 1.1471 1.1471 1.1471 0 0 1-1.1471-1.1471 1.1471 1.1471 0 0 1 1.1471-1.1471z"
          fill="#e7e3c4"
        />
        <path
          d="m23.181 1.1802a1.6229 1.6229 0 0 0-1.6198 1.6229 1.6229 1.6229 0 0 0 1.6228 1.6228 1.6229 1.6229 0 0 0 1.6229-1.6228 1.6229 1.6229 0 0 0-1.6229-1.6229 1.6229 1.6229 0 0 0-0.0031 0zm0.0031 0.43845a1.1471 1.1471 0 0 1 1.1471 1.1471 1.1471 1.1471 0 0 1-1.1471 1.1471 1.1471 1.1471 0 0 1-1.1471-1.1471 1.1471 1.1471 0 0 1 1.1471-1.1471z"
          fill="#e7e3c4"
        />
        <path
          d="m15.049 3.0132c-0.14489 0.02316-0.26986-0.0058-0.27922-0.06459-0.0094-0.05874 0.1005-0.1251 0.24541-0.1481 0.14481-0.023 0.26976 0.0058 0.27913 0.06451 0.0094 0.05874-0.1004 0.12518-0.24531 0.14818m-0.1376 0.60509c-0.05307 0.027-0.1501-0.05691-0.21671-0.18746-0.06668-0.13072-0.07782-0.2587-0.02468-0.2857 0.0529-0.02693 0.14978 0.05697 0.21654 0.18761 0.06668 0.13054 0.0779 0.25845 0.02485 0.28555m-0.57077-0.24323c-0.10363 0.10379-0.22167 0.1538-0.26376 0.11171-0.04214-0.04199 0.0078-0.16022 0.1114-0.26399 0.10354-0.10394 0.22158-0.15395 0.26384-0.11189 0.04206 0.04201-0.0078 0.1603-0.11148 0.26416m-0.39663-0.72272c0.02685-0.05307 0.15476-0.04201 0.2853 0.02443 0.13079 0.06645 0.21504 0.16341 0.18802 0.21638-0.027 0.05289-0.15476 0.04209-0.28545-0.02435-0.13072-0.06643-0.21487-0.16341-0.18786-0.21646m0.75238-0.38413c0.05882 0.0091 0.0879 0.13424 0.06492 0.27913-0.0227 0.14491-0.08873 0.25478-0.14762 0.24556-0.05866-0.0093-0.08784-0.13425-0.06509-0.27905 0.02285-0.14491 0.08896-0.25485 0.1478-0.24564m1.8552 0.39655c-0.01218-0.07703-0.05401-0.09906-0.15492-0.13127-0.07663-0.02462-0.58986-0.18811-0.58986-0.18811s-0.35776-0.1352-0.62286 0.05595c-0.03575 0.02579-0.07278 0.05512-0.10988 0.08638 0.03118-0.03718 0.06051-0.07413 0.08625-0.10987 0.19057-0.26551 0.05482-0.62311 0.05482-0.62311s-0.16438-0.51284-0.18915-0.5896c-0.03245-0.10067-0.05449-0.14251-0.1316-0.15445-0.07724-0.01225-0.1109 0.0207-0.17278 0.1066-0.04713 0.06524-0.36128 0.5029-0.36128 0.5029s-0.23885 0.29846-0.13922 0.6098c0.01352 0.04176 0.02998 0.08607 0.04834 0.13112-0.02582-0.0412-0.05193-0.08053-0.07798-0.11598-0.19371-0.26314-0.5755-0.24479-0.5755-0.24479s-0.53864-0.0021-0.61911-0.0022c-0.10594-3.55e-4 -0.15243 0.0079-0.18784 0.07734-0.03543 0.06949-0.01451 0.11194 0.04809 0.19723 0.04751 0.06501 0.3668 0.4989 0.3668 0.4989s0.20998 0.3196 0.53673 0.32088c0.04391 2.18e-4 0.09096-0.0018 0.13928-0.0053a1.9307 1.9307 0 0 0-0.13406 0.03824c-0.31025 0.1029-0.4105 0.47187-0.4105 0.47187s-0.16847 0.51163-0.19363 0.58824c-0.03285 0.1005-0.03951 0.14722 0.01563 0.20228 0.0552 0.0553 0.10202 0.04847 0.20252 0.01532 0.07653-0.02508 0.58791-0.19459 0.58791-0.19459s0.36858-0.10106 0.47076-0.41146c0.01388-0.04166 0.02645-0.08704 0.03816-0.13383-0.0035 0.04824-0.0053 0.09521-0.0049 0.13912 0.0018 0.32683 0.32169 0.53625 0.32169 0.53625s0.43436 0.31815 0.49943 0.36576c0.08559 0.06228 0.12791 0.08319 0.19748 0.0477 0.0694-0.03543 0.07751-0.08192 0.07701-0.18786-1.97e-4 -0.08048-0.0035-0.61911-0.0035-0.61911s0.01794-0.38188-0.24564-0.5751c-0.03543-0.02596-0.07463-0.05201-0.11579-0.07765 0.04495 0.0181 0.08902 0.03454 0.13102 0.04768 0.31134 0.09929 0.60947-0.14018 0.60947-0.14018s0.43694-0.31486 0.50226-0.36199c0.08575-0.06203 0.11852-0.09609 0.10634-0.17311"
          fill="#fff"
        />
        <text fill="#fff" font-size="2.5px" font-weight="bold">
          <tspan x="12.6" y="12.7">+</tspan>
        </text>
      </svg>
    `}};v2=d3([U("wokwi-ds1307")],v2);var w1=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const Gi=3,O1=6;let zt=class extends L{constructor(){super(...arguments);this.pixels=16,this.pixelSpacing=0,this.background="#363",this.animation=!1,this.pixelElements=null,this.animationFrame=null,this.animateStep=()=>{const e=new Date().getTime(),{pixels:i}=this,s=n=>n%2e3>1e3?1-n%1e3/1e3:n%1e3/1e3;for(let n=0;n<i;n++)this.setPixel(n,{r:s(n*100+e),g:s(n*100+e+200),b:s(n*100+e+400)});this.animationFrame=requestAnimationFrame(this.animateStep)}}get radius(){return(this.pixelSpacing+5)*this.pixels/2/Math.PI+O1}get pinInfo(){const{radius:e}=this,i=2.54,s=(e*2+Gi)*Qe,n=e*Qe,r=i*Qe;return[{name:"GND",x:n-1.5*r,y:s,signals:[{type:"power",signal:"GND"}]},{name:"VCC",x:n-.5*r,y:s,signals:[{type:"power",signal:"VCC"}]},{name:"DIN",x:n+.5*r,y:s,signals:[]},{name:"DOUT",x:n+1.5*r,y:s,signals:[]}]}getPixelElements(){return this.shadowRoot?(this.pixelElements||(this.pixelElements=Array.from(this.shadowRoot.querySelectorAll("rect.pixel"))),this.pixelElements):null}setPixel(e,{r:i,g:s,b:n}){const r=this.getPixelElements();!r||e<0||e>=r.length||(r[e].style.fill=`rgb(${i*255},${s*255},${n*255})`)}reset(){const e=this.getPixelElements();for(const i of e!=null?e:[])i.style.fill=""}updated(){this.animation&&!this.animationFrame?this.animationFrame=requestAnimationFrame(this.animateStep):!this.animation&&this.animationFrame&&(cancelAnimationFrame(this.animationFrame),this.animationFrame=null)}render(){const{pixels:e,radius:i,background:s}=this,n=[],r=i*2,l=i*2+Gi;for(let o=0;o<e;o++){const a=o/e*360;n.push(V`<rect
          class="pixel"
          x="${i-2.5}"
          y="${O1/2-2.5}"
          width="5"
          height="5"
          fill="white"
          stroke="black"
          stroke-width="0.25"
          transform="rotate(${a} ${i} ${i})"/>`)}return this.pixelElements=null,G`
      <svg
        width="${r}mm"
        height="${l}mm"
        version="1.1"
        viewBox="0 0 ${r} ${l}"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="2" width="2.54" patternUnits="userSpaceOnUse">
            <rect x="1.02" y="0" height="2" width="0.5" fill="#aaa" />
          </pattern>
        </defs>
        <rect
          fill="url(#pin-pattern)"
          height="${Gi+1}"
          width=${4*2.54}
          transform="translate(${i-4*2.54/2}, ${i*2-1})"
        />
        <circle
          cx="${i}"
          cy="${i}"
          r="${i-O1/2}"
          fill="transparent"
          stroke-width="${O1}"
          stroke="${s}"
        />
        ${n}
      </svg>
    `}};w1([y()],zt.prototype,"pixels",void 0);w1([y({type:Number})],zt.prototype,"pixelSpacing",void 0);w1([y()],zt.prototype,"background",void 0);w1([y()],zt.prototype,"animation",void 0);zt=w1([U("wokwi-led-ring")],zt);var kl=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let ws=class extends L{constructor(){super(...arguments);this.value=0,this.pinInfo=[{name:"1",number:1,y:34,x:6.5,signals:[]},{name:"2",number:2,y:34,x:16,signals:[]},{name:"3",number:3,y:34,x:25.5,signals:[]}]}static get styles(){return ke`
      .hide-input {
        position: absolute;
        clip: rect(0 0 0 0);
        width: 1px;
        height: 1px;
        margin: -1px;
      }
      svg #handle {
        transition: transform 0.2s linear;
      }
      input:checked + svg #handle {
        transform: translate(2px, 0);
      }
      input:focus + svg #handle {
        stroke-width: 0.4px;
        stroke: #8080ff;
      }
    `}onClick(){var e;const i=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector(".hide-input");i&&(i.checked=!i.checked,this.onValueChange(i),i==null||i.focus())}onValueChange(e){this.value=e.checked?1:0,this.dispatchEvent(new InputEvent("input",{detail:this.value}))}render(){const{value:e}=this;return G`
      <input
        tabindex="0"
        type="checkbox"
        class="hide-input"
        .checked=${e}
        @input="${i=>this.onValueChange(i.target)}"
      />
      <svg
        width="8.5mm"
        height="9.23mm"
        version="1.1"
        viewBox="0 0 8.5 9.23"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
        @click="${this.onClick}"
      >
        <defs>
          <radialGradient
            id="a"
            cx="9.33"
            cy="122"
            r="4.25"
            gradientTransform="matrix(1.75 -.511 .28 .959 -41.2 8.15)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#808080" offset="0" />
            <stop stop-color="#b5b5b5" offset="1" />
          </radialGradient>
        </defs>
        <g fill="#aaa" stroke-width=".0673">
          <rect x="4" y="5" width=".5" height="4.2" rx=".25" ry=".25" />
          <rect x="1.54" y="5" width=".5" height="4.2" rx=".25" ry=".25" />
          <rect x="6.5" y="5" width=".5" height="4.2" rx=".25" ry=".25" />
        </g>
        <path
          id="handle"
          d="m2.74 0.128 0.145-0.128 0.177 0.0725 0.174-0.0725 0.151 0.0725 0.154-0.0725 0.151 0.0725 0.128-0.0725 0.134 0.0725 0.123-0.0725 0.145 0.128 2e-5 2h-1.48z"
          stroke-width=".0623"
        />
        <rect x="0" y="2.06" width="8.5" height="3.48" fill="url(#a)" stroke-width=".0548" />
        <rect x=".0322" y="4.74" width="1.55" height=".805" stroke-width=".0637" />
        <rect x="6.95" y="4.74" width="1.55" height=".805" stroke-width=".0637" />
        <rect x="2.55" y="4.74" width="3.47" height=".805" stroke-width=".0955" />
      </svg>
    `}};kl([y()],ws.prototype,"value",void 0);ws=kl([U("wokwi-slide-switch")],ws);var p3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let w2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"VCC",x:71.78,y:94.5,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"TRIG",x:79.67,y:94.5,signals:[]},{name:"ECHO",x:87.56,y:94.5,signals:[]},{name:"GND",x:95.45,y:94.5,signals:[{type:"power",signal:"GND"}]}]}render(){return G`
      <svg
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
        viewBox="0 0 45 25"
        height="25mm"
        width="45mm"
        font-family="monospace"
      >
        <defs>
          <pattern patternUnits="userSpaceOnUse" width="2" height="2" id="checkerboard">
            <path d="M0 0h1v1H0zM1 1h1v1H1z" />
          </pattern>
          <radialGradient id="grad1" cx="8.96" cy="10.04" r="3.58" gradientUnits="userSpaceOnUse">
            <stop stop-color="#777" offset="0" />
            <stop stop-color="#b9b9b9" offset="1" />
          </radialGradient>
          <g id="sensor-unit">
            <circle cx="8.98" cy="10" r="8.61" fill="#dcdcdc" />
            <circle cx="8.98" cy="10" r="7.17" fill="#222" />
            <circle cx="8.98" cy="10" r="5.53" fill="#777" fill-opacity=".992" />
            <circle cx="8.98" cy="10" r="3.59" fill="url(#grad1)" />
            <circle cx="8.99" cy="10" r=".277" fill="#777" fill-opacity=".818" />
            <circle cx="8.98" cy="10" r="5.53" fill="url(#checkerboard)" opacity=".397" />
          </g>
        </defs>
        <path
          d="M0 0v20.948h45V0zm1.422.464a1 1 0 01.004 0 1 1 0 011 1 1 1 0 01-1 1 1 1 0 01-1-1 1 1 0 01.996-1zm41.956 0a1 1 0 01.004 0 1 1 0 011 1 1 1 0 01-1 1 1 1 0 01-1-1 1 1 0 01.996-1zM1.422 18.484a1 1 0 01.004 0 1 1 0 011 1 1 1 0 01-1 1 1 1 0 01-1-1 1 1 0 01.996-1zm41.956 0a1 1 0 01.004 0 1 1 0 011 1 1 1 0 01-1 1 1 1 0 01-1-1 1 1 0 01.996-1z"
          fill="#456f93"
        />
        <path
          d="M15.293 5.888l2.934-2.934v3.124l2.944 2.943v10.143M23.269 19.037v-2.473l-.966-.965v-12.5l2.577 1.488 4.741 4.741"
          fill="none"
          stroke="#355a7c"
          stroke-width=".858"
        />
        <use xlink:href="#sensor-unit" />
        <use xlink:href="#sensor-unit" x="27.12" />
        <g fill="none" stroke="#505132" stroke-width=".368">
          <circle cx="43.4" cy="1.46" r="1" />
          <circle cx="43.4" cy="19.5" r="1" />
          <circle cx="1.43" cy="1.46" r="1" />
          <circle cx="1.43" cy="19.5" r="1" />
        </g>
        <rect
          ry="2.07"
          y=".626"
          x="17.111"
          height="4.139"
          width="10.272"
          fill="#878787"
          stroke="#424242"
          stroke-width=".368"
        />
        <g fill="black">
          <rect x="17.87" y="18" ry=".568" width="2.25" height="2.271" />
          <rect x="19.95" y="18" ry=".568" width="2.25" height="2.271" />
          <rect x="22.04" y="18" ry=".568" width="2.25" height="2.271" />
          <rect x="24.12" y="18" ry=".568" width="2.25" height="2.271" />
        </g>
        <g fill="#ccc" stroke-linecap="round" stroke-width=".21">
          <rect x="18.616" y="19" width=".75" height="7" rx=".2" />
          <rect x="20.702" y="19" width=".75" height="7" rx=".2" />
          <rect x="22.789" y="19" width=".75" height="7" rx=".2" />
          <rect x="24.875" y="19" width=".75" height="7" rx=".2" />
        </g>
        <text font-weight="400" font-size="2.2" fill="#e6e6e6" stroke-width=".055">
          <tspan y="8" x="18">HC-SR04</tspan>
        </text>
        <text
          transform="rotate(-90)"
          font-weight="400"
          font-size="1.55"
          fill="#e6e6e6"
          stroke-width=".039"
        >
          <tspan x="-17.591" y="19.561">Vcc</tspan>
          <tspan x="-17.591" y="21.654">Trig</tspan>
          <tspan x="-17.591" y="23.747">Echo</tspan>
          <tspan x="-17.591" y="25.84">Gnd</tspan>
        </text>
      </svg>
    `}};w2=p3([U("wokwi-hc-sr04")],w2);var u3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let b2=class extends Fe{constructor(){super(...arguments);this.numCols=20,this.numRows=4}};b2=u3([U("wokwi-lcd2004")],b2);var b1=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let _t=class extends L{constructor(){super(...arguments);this.xValue=0,this.yValue=0,this.pressed=!1,this.pinInfo=[{name:"VCC",x:33,y:115.8,signals:[De()]},{name:"VERT",x:42.6012,y:115.8,signals:[H(0)]},{name:"HORZ",x:52.2024,y:115.8,signals:[H(1)]},{name:"SEL",x:61.8036,y:115.8,signals:[]},{name:"GND",x:71.4048,y:115.8,signals:[$e()]}]}static get styles(){return ke`
      #knob {
        transition: transform 0.3s;
      }

      #knob:hover {
        fill: #222;
      }

      #knob:focus {
        outline: none;
        stroke: #4d90fe;
        stroke-width: 0.5;
      }

      .controls {
        opacity: 0;
        transition: opacity 0.3s;
        cursor: pointer;
      }

      #knob:focus ~ .controls,
      #knob:hover ~ .controls {
        opacity: 1;
      }

      .controls:hover {
        opacity: 1;
      }

      .controls path {
        pointer-events: none;
      }

      .controls .region {
        pointer-events: fill;
        fill: none;
      }

      .controls .region:hover + path {
        fill: #fff;
      }

      .controls circle:hover {
        stroke: #fff;
      }

      .controls circle.pressed {
        fill: #fff;
      }
    `}render(){const{xValue:e,yValue:i}=this;return G`
      <svg
        width="27.2mm"
        height="31.8mm"
        viewBox="0 0 27.2 31.8"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <filter id="noise" primitiveUnits="objectBoundingBox">
            <feTurbulence baseFrequency="2 2" type="fractalNoise" />
            <feColorMatrix
              values=".1 0 0 0 .1
                      .1 0 0 0 .1
                      .1 0 0 0 .1
                      0 0 0 0 1"
            />
            <feComposite in2="SourceGraphic" operator="lighter" />
            <feComposite result="body" in2="SourceAlpha" operator="in" />
          </filter>
          <radialGradient id="g-knob" cx="13.6" cy="13.6" r="10.6" gradientUnits="userSpaceOnUse">
            <stop offset="0" />
            <stop offset="0.9" />
            <stop stop-color="#777" offset="1" />
          </radialGradient>
          <radialGradient
            id="g-knob-base"
            cx="13.6"
            cy="13.6"
            r="13.6"
            gradientUnits="userSpaceOnUse"
          >
            <stop offset="0" />
            <stop stop-color="#444" offset=".8" />
            <stop stop-color="#555" offset=".9" />
            <stop offset="1" />
          </radialGradient>
          <path
            id="pin"
            fill="silver"
            stroke="#a2a2a2"
            stroke-width=".024"
            d="M8.726 29.801a.828.828 0 00-.828.829.828.828 0 00.828.828.828.828 0 00.829-.828.828.828 0 00-.829-.829zm-.004.34a.49.49 0 01.004 0 .49.49 0 01.49.489.49.49 0 01-.49.49.49.49 0 01-.489-.49.49.49 0 01.485-.49z"
          />
        </defs>
        <path
          d="M1.3 0v31.7h25.5V0zm2.33.683a1.87 1.87 0 01.009 0 1.87 1.87 0 011.87 1.87 1.87 1.87 0 01-1.87 1.87 1.87 1.87 0 01-1.87-1.87 1.87 1.87 0 011.87-1.87zm20.5 0a1.87 1.87 0 01.009 0 1.87 1.87 0 011.87 1.87 1.87 1.87 0 01-1.87 1.87 1.87 1.87 0 01-1.87-1.87 1.87 1.87 0 011.87-1.87zm-20.5 26.8a1.87 1.87 0 01.009 0 1.87 1.87 0 011.87 1.87 1.87 1.87 0 01-1.87 1.87 1.87 1.87 0 01-1.87-1.87 1.87 1.87 0 011.87-1.87zm20.4 0a1.87 1.87 0 01.009 0 1.87 1.87 0 011.87 1.87 1.87 1.87 0 01-1.87 1.87 1.87 1.87 0 01-1.87-1.87 1.87 1.87 0 011.87-1.87zm-12.7 2.66a.489.489 0 01.004 0 .489.489 0 01.489.489.489.489 0 01-.489.489.489.489 0 01-.489-.489.489.489 0 01.485-.489zm2.57 0a.489.489 0 01.004 0 .489.489 0 01.489.489.489.489 0 01-.489.489.489.489 0 01-.489-.489.489.489 0 01.485-.489zm2.49.013a.489.489 0 01.004 0 .489.489 0 01.489.489.489.489 0 01-.489.489.489.489 0 01-.489-.489.489.489 0 01.485-.489zm-7.62.007a.489.489 0 01.004 0 .489.489 0 01.489.489.489.489 0 01-.489.489.489.489 0 01-.489-.49.489.489 0 01.485-.488zm10.2.013a.489.489 0 01.004 0 .489.489 0 01.489.489.489.489 0 01-.489.489.489.489 0 01-.489-.49.489.489 0 01.485-.488z"
          fill="#bd1e34"
        />
        <g fill="#fff" font-family="sans-serif" stroke-width=".03">
          <text text-anchor="middle" font-size="1.2" letter-spacing=".053">
            <tspan x="4.034" y="25.643">Analog</tspan>
            <tspan x="4.061" y="27.159">Joystick</tspan>
          </text>
          <text transform="rotate(-90)" text-anchor="start" font-size="1.2">
            <tspan x="-29.2" y="9.2">VCC</tspan>
            <tspan x="-29.2" y="11.74">VERT</tspan>
            <tspan x="-29.2" y="14.28">HORZ</tspan>
            <tspan x="-29.2" y="16.82">SEL</tspan>
            <tspan x="-29.2" y="19.36">GND</tspan>
          </text>
        </g>
        <ellipse cx="13.6" cy="13.7" rx="13.6" ry="13.7" fill="url(#g-knob-base)" />
        <path
          d="M48.2 65.5s.042.179-.093.204c-.094.017-.246-.077-.322-.17-.094-.115-.082-.205-.009-.285.11-.122.299-.075.299-.075s-.345-.303-.705-.054c-.32.22-.228.52.06.783.262.237.053.497-.21.463-.18-.023-.252-.167-.21-.256.038-.076.167-.122.167-.122s-.149-.06-.324.005c-.157.06-.286.19-.276.513v1.51s.162-.2.352-.403c.214-.229.311-.384.53-.366.415.026.714-.159.918-.454.391-.569.085-1.2-.178-1.29"
          fill="#fff"
        />
        <circle
          id="knob"
          cx="13.6"
          cy="13.6"
          transform="translate(${2.5*-e}, ${2.5*-i})"
          r="10.6"
          fill="url(#g-knob)"
          filter="url(#noise)"
          tabindex="0"
          @keyup=${s=>this.keyup(s)}
          @keydown=${s=>this.keydown(s)}
        />
        <g fill="none" stroke="#fff" stroke-width=".142">
          <path
            d="M7.8 31.7l-.383-.351v-1.31l.617-.656h1.19l.721.656.675-.656h1.18l.708.656.662-.656h1.25l.643.656.63-.656h1.21l.695.656.636-.656h1.17l.753.656v1.3l-.416.39"
          />
          <path
            d="M9.5 31.7l.381-.344.381.331M12.1 31.7l.381-.344.381.331M14.7 31.7l.381-.344.381.331M17.2 31.7l.381-.344.381.331"
            stroke-linecap="square"
            stroke-linejoin="bevel"
          />
        </g>
        <g class="controls" stroke-width="0.6" stroke-linejoin="bevel" fill="#aaa">
          <rect
            class="region"
            y="8.5"
            x="1"
            height="10"
            width="7"
            @mousedown=${s=>this.mousedown(s,1,0)}
            @mouseup=${()=>this.mouseup(!0,!1)}
          />
          <path d="m 7.022,11.459 -3.202,2.497 3.202,2.497" />

          <rect
            class="region"
            y="1.38"
            x="7.9"
            height="7"
            width="10"
            @mousedown=${s=>this.mousedown(s,0,1)}
            @mouseup=${()=>this.mouseup(!1,!0)}
          />
          <path d="m 16.615,7.095 -2.497,-3.202 -2.497,3.202" />

          <rect
            class="region"
            y="8.5"
            x="18"
            height="10"
            width="7"
            @mousedown=${s=>this.mousedown(s,-1,0)}
            @mouseup=${()=>this.mouseup(!0,!1)}
          />
          <path d="m 19.980,16.101 3.202,-2.497 -3.202,-2.497" />

          <rect
            class="region"
            y="17"
            x="7.9"
            height="7"
            width="10"
            @mousedown=${s=>this.mousedown(s,0,-1)}
            @mouseup=${()=>this.mouseup(!1,!0)}
          />
          <path d="m 11.620,20.112 2.497,3.202 2.497,-3.202" />

          <circle
            cx="13.6"
            cy="13.6"
            r="3"
            stroke="#aaa"
            class=${this.pressed?"pressed":""}
            @mousedown=${s=>this.press(s)}
            @mouseup=${()=>this.release()}
          />
        </g>
        <use xlink:href="#pin" x="0" />
        <use xlink:href="#pin" x="2.54" />
        <use xlink:href="#pin" x="5.08" />
        <use xlink:href="#pin" x="7.62" />
        <use xlink:href="#pin" x="10.16" />
      </svg>
    `}keydown(e){switch(e.key){case"ArrowUp":this.yValue=1,this.valueChanged();break;case"ArrowDown":this.yValue=-1,this.valueChanged();break;case"ArrowLeft":this.xValue=1,this.valueChanged();break;case"ArrowRight":this.xValue=-1,this.valueChanged();break}Te.includes(e.key)&&this.press()}keyup(e){switch(e.key){case"ArrowUp":case"ArrowDown":this.yValue=0,this.valueChanged();break;case"ArrowLeft":case"ArrowRight":this.xValue=0,this.valueChanged();break}Te.includes(e.key)&&this.release()}mousedown(e,i,s){var n;i&&(this.xValue=i),s&&(this.yValue=s),this.valueChanged(),(n=this.knob)===null||n===void 0||n.focus(),e.preventDefault()}mouseup(e,i){var s;e&&(this.xValue=0),i&&(this.yValue=0),this.valueChanged(),(s=this.knob)===null||s===void 0||s.focus()}press(e){var i;this.pressed=!0,this.dispatchEvent(new InputEvent("button-press")),(i=this.knob)===null||i===void 0||i.focus(),e==null||e.preventDefault()}release(){var e;this.pressed=!1,this.dispatchEvent(new InputEvent("button-release")),(e=this.knob)===null||e===void 0||e.focus()}valueChanged(){this.dispatchEvent(new InputEvent("input"))}};b1([y({type:Number})],_t.prototype,"xValue",void 0);b1([y({type:Number})],_t.prototype,"yValue",void 0);b1([y()],_t.prototype,"pressed",void 0);b1([v1("#knob")],_t.prototype,"knob",void 0);_t=b1([U("wokwi-analog-joystick")],_t);var Ot=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let Z0=class extends L{constructor(){super(...arguments);this.travelLength=30,this.value=0,this.min=0,this.max=100,this.step=2,this.isPressed=!1,this.zoom=1,this.pageToLocalTransformationMatrix=null,this.up=()=>{this.isPressed&&(this.isPressed=!1)},this.mouseMove=e=>{this.isPressed&&this.updateValueFromXCoordinate(new DOMPointReadOnly(e.pageX,e.pageY))}}get pinInfo(){return[{name:"VCC",x:1,y:43,number:1,signals:[{type:"power",signal:"VCC"}]},{name:"SIG",x:1,y:66.5,number:2,signals:[H(0)]},{name:"GND",x:93.6+this.travelLength*Qe,y:43,number:3,signals:[{type:"power",signal:"GND"}]}]}static get styles(){return ke`
      .hide-input {
        position: absolute;
        clip: rect(0 0 0 0);
        width: 1px;
        height: 1px;
        margin: -1px;
      }
      input:focus + svg #tip {
        /* some style to add when the element has focus */
        filter: url(#outline);
      }
    `}render(){const{value:e,min:i,max:s,travelLength:n}=this,r=-15,o=e/(s-i)*n+r;return G`
      <input
        tabindex="0"
        type="range"
        min="${this.min}"
        max="${this.max}"
        value="${this.value}"
        step="${this.step}"
        aria-valuemin="${this.min}"
        aria-valuenow="${this.value}"
        aria-valuemax="${this.max}"
        @input="${this.onInputValueChange}"
        class="hide-input"
      />
      <svg
        width="${n+25}mm"
        height="29mm"
        version="1.1"
        viewBox="0 0 ${n+25} 29"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <filter id="outline">
            <feDropShadow dx="0" dy="0" stdDeviation="1" flood-color="#4faaff" />
          </filter>
          <linearGradient
            id="tipGradient"
            x1="36.482"
            x2="50.447"
            y1="91.25"
            y2="91.25"
            gradientTransform="matrix(.8593 0 0 1.1151 -14.849 -92.256)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#1a1a1a" offset="0" />
            <stop stop-color="#595959" offset=".4" />
            <stop stop-color="#595959" offset=".6" />
            <stop stop-color="#1a1a1a" offset="1" />
          </linearGradient>
          <radialGradient
            id="bodyGradient"
            cx="62.59"
            cy="65.437"
            r="22.5"
            gradientTransform="matrix(1.9295 3.7154e-8 0 .49697 -98.268 -23.02)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#d2d2d2" offset="0" />
            <stop stop-color="#7a7a7a" offset="1" />
          </radialGradient>
          <g id="screw">
            <circle cx="0" cy="0" r="1" fill="#858585" stroke="#000" stroke-width=".05" />
            <path d="m0 1 0-2" fill="none" stroke="#000" stroke-width=".151" />
          </g>
        </defs>
        <!-- pins -->
        <g fill="#ccc">
          <rect x="0" y="11" width="5" height="0.75" />
          <rect x="${n+20}" y="11" width="5" height="0.75" />
          <rect x="0" y="17.25" width="5" height="0.75" />
        </g>
        <g transform="translate(5 5)">
          <!-- Body -->
          <rect
            id="sliderCase"
            x="0"
            y="5"
            width="${n+15}"
            height="9"
            rx=".2"
            ry=".2"
            fill="url(#bodyGradient)"
            fill-rule="evenodd"
          />
          <rect
            x="3.25"
            y="8"
            width="${n+8.5}"
            height="3"
            rx=".1"
            ry=".1"
            fill="#3f1e1e"
          />
          <!-- Screw Left -->
          <g transform="translate(1.625 9.5) rotate(45)">
            <use href="#screw" />
          </g>
          <!-- Screw Right -->
          <g transform="translate(${n+13.375} 9.5) rotate(45)">
            <use href="#screw" />
          </g>
          <!-- Tip -->
          <g
            id="tip"
            transform="translate(${o} 0)"
            @mousedown=${this.down}
            @touchstart=${this.down}
            @touchmove=${this.touchMove}
            @touchend=${this.up}
            @keydown=${this.down}
            @keyup=${this.up}
            @click="${this.focusInput}"
          >
            <rect x="19.75" y="8.6" width="5.5" height="1.8" />
            <rect
              x="16.5"
              y="0"
              width="12"
              height="19"
              fill="url(#tipGradient)"
              stroke-width="2.6518"
              rx=".1"
              ry=".1"
            />
            <rect x="22.2" y="0" width=".6" height="19" fill="#efefef" />
          </g>
        </g>
      </svg>
    `}connectedCallback(){super.connectedCallback(),window.addEventListener("mouseup",this.up),window.addEventListener("mousemove",this.mouseMove),window.addEventListener("mouseleave",this.up)}disconnectedCallback(){super.disconnectedCallback(),window.removeEventListener("mouseup",this.up),window.removeEventListener("mousemove",this.mouseMove),window.removeEventListener("mouseleave",this.up)}focusInput(){var e;const i=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector(".hide-input");i==null||i.focus()}down(){this.isPressed||this.updateCaseDisplayProperties(),this.isPressed=!0}updateCaseDisplayProperties(){var e,i,s;const n=(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector("#sliderCase");n&&(this.pageToLocalTransformationMatrix=((i=n.getScreenCTM())===null||i===void 0?void 0:i.inverse())||null);const r=(s=getComputedStyle(window.document.body))===null||s===void 0?void 0:s.zoom;r!==void 0?this.zoom=Number(r):this.zoom=1}onInputValueChange(e){const i=e.target;i.value&&this.updateValue(Number(i.value))}touchMove(e){if(this.isPressed&&e.targetTouches.length>0){const i=e.targetTouches[0];this.updateValueFromXCoordinate(new DOMPointReadOnly(i.pageX,i.pageY))}}updateValueFromXCoordinate(e){if(this.pageToLocalTransformationMatrix){let i=new DOMPointReadOnly(e.x/this.zoom,e.y/this.zoom);i=i.matrixTransform(this.pageToLocalTransformationMatrix);const s=7.5,n=i.x-s,r=this.travelLength/(this.max-this.min);this.updateValue(Math.round(n/r))}}updateValue(e){this.value=L1(this.min,this.max,e),this.dispatchEvent(new InputEvent("input",{detail:this.value}))}};Ot([y({type:Number})],Z0.prototype,"travelLength",void 0);Ot([y({type:Number})],Z0.prototype,"value",void 0);Ot([y({type:Number})],Z0.prototype,"min",void 0);Ot([y({type:Number})],Z0.prototype,"max",void 0);Ot([y({type:Number})],Z0.prototype,"step",void 0);Z0=Ot([U("wokwi-slide-potentiometer")],Z0);var m3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let k2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",y:87.75,x:20.977,number:1,signals:[$e()]},{name:"VCC",y:87.75,x:30.578,number:2,signals:[De()]},{name:"DAT",y:87.75,x:40.18,number:3,signals:[]}]}render(){return G`
      <svg
        version="1.1"
        viewBox="0 0 61.1 88.7"
        width="16.178mm"
        height="23.482mm"
        font-family="sans-serif"
        xmlns="http://www.w3.org/2000/svg"
      >
        <g fill="#171514">
          <path
            d="m61.1 4.85c0-2.68-2.17-4.85-4.85-4.85h-51.4c-2.68 0-4.85 2.17-4.85 4.85v61c0 2.68 2.17 4.85 4.85 4.85h51.4c2.68 0 4.85-2.17 4.85-4.85zm-7.43 53.3c2.29 0 4.14 1.86 4.14 4.14 0 2.28-1.85 4.14-4.14 4.14s-4.14-1.86-4.14-4.14c0-2.29 1.85-4.14 4.14-4.14zm-46.3 0c2.29 0 4.14 1.86 4.14 4.14 0 2.28-1.85 4.14-4.14 4.14-2.29 0-4.14-1.86-4.14-4.14 0-2.29 1.85-4.14 4.14-4.14z"
            stroke-width=".987"
          />
          <rect x="16.5" y="58.2" width="28.2" height="8.28" stroke="#fff" stroke-width=".888px" />
          <rect x="14.2" y="23" width="11.3" height="4.66" stroke="#fff" stroke-width=".888px" />
        </g>
        <rect x="15.2" y="23.7" width="9.44" height="3.23" fill="#a19e9e" stroke-width=".987" />
        <g fill="#171514" stroke="#fff" stroke-width=".888px">
          <rect x="14.2" y="33" width="11.3" height="4.66" />
          <rect x="31.6" y="23" width="11.3" height="4.66" />
          <rect x="31.6" y="33" width="11.3" height="4.66" />
        </g>
        <g fill="#433b38" stroke-width=".987">
          <rect x="17.7" y="59.1" width="6.47" height="6.47" />
          <rect x="27.3" y="59.1" width="6.47" height="6.47" />
          <rect x="37" y="59.1" width="6.47" height="6.47" />
        </g>
        <g fill="#9f9f9f" stroke-width=".987">
          <path
            d="m22.4 62.5c0-0.377-0.149-0.739-0.416-1.01-0.268-0.267-0.629-0.417-1.01-0.417-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.418 0.419h2.01c0.231 0 0.418-0.188 0.418-0.419v-25.8z"
          />
          <path
            d="m32 62.5c0-0.377-0.149-0.739-0.416-1.01-0.268-0.267-0.629-0.417-1.01-0.417-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.418 0.419h2.01c0.231 0 0.418-0.188 0.418-0.419v-25.8z"
          />
          <path
            d="m41.6 62.5c0-0.377-0.15-0.739-0.417-1.01s-0.629-0.417-1.01-0.417c-0.377 0-0.739 0.15-1.01 0.417s-0.417 0.629-0.417 1.01v25.8c0 0.231 0.188 0.419 0.419 0.419h2.01c0.231 0 0.419-0.188 0.419-0.419v-25.8z"
          />
        </g>
        <text transform="rotate(90)" fill="#ffffff" font-size="5px">
          <tspan x="45.369" y="-37.601">DAT</tspan>
          <tspan x="45.609" y="-28.801">VCC</tspan>
          <tspan x="45.359" y="-20.2">GND</tspan>
          <text font-size="5.71px">
            <tspan
              x="16.234 18.076 22.422 24.263 28.608 32.018 35.112 36.639 40.05 43.144 46.553"
              y="-52.266"
            >
              IR Reciever
            </tspan>
          </text>
        </text>
        <g fill="none" stroke="#fff">
          <path
            d="m56.3 6.32c-0.654 0.514-1.48 0.82-2.37 0.82-0.895 0-1.72-0.306-2.37-0.82"
            stroke-width=".316px"
          />
          <path
            d="m57.4 7.97c-0.949 0.745-2.14 1.19-3.44 1.19-1.3 0-2.49-0.445-3.44-1.19"
            stroke-width=".395px"
          />
          <path
            d="m58.9 9.32c-1.38 1.08-3.11 1.73-5 1.73s-3.62-0.646-5-1.73"
            stroke-width=".395px"
          />
        </g>
        <path
          d="m20.4 10.2h-6.13c-0.382 0-0.691 0.309-0.691 0.691v6.2c0 0.382 0.309 0.691 0.691 0.691h13c0.931 0.0563 1.88 0.0563 2.81 0h12.7c0.381 0 0.691-0.309 0.691-0.691v-6.2c0-0.382-0.31-0.691-0.691-0.691h-5.88c-1.39-3.12-4.55-5.31-8.23-5.31-3.68 0-6.84 2.19-8.23 5.31zm0.463 0.691c1.18-3.1 4.21-5.31 7.77-5.31 3.55 0 6.59 2.21 7.76 5.31h6.35v6.2h-12.7c-0.914 0.0563-1.85 0.0563-2.77 0h-13v-6.2z"
          fill="#fff"
          stroke-width=".987"
        />
        <path
          d="m28.6 6.32c4.01 0 7.27 3.26 7.27 7.27 0 4.01-14.5 4.01-14.5 0 0-4.01 3.26-7.27 7.27-7.27z"
          fill="#2d2624"
          stroke-width=".987"
        />
        <clipPath id="b">
          <path
            d="m37.2 14.5c4.06 0 7.36 3.3 7.36 7.36 0 4.06-14.7 4.06-14.7 0 0-4.06 3.3-7.36 7.36-7.36z"
          />
        </clipPath>
        <g transform="matrix(.987 0 0 .987 -8.13 -8.03)" clip-path="url(#b)">
          <path
            d="m37.2 12.3c-0.069 0.303 0.377 0.714 0.536 0.965 0.504 0.799 0.744 1.43 1.07 2.3 1.01 2.7 0.775 5.41 0.775 8.2 0 0.121 0.155-0.196 0.262-0.254 0.233-0.126 0.484-0.232 0.724-0.345 0.727-0.341 1.47-0.602 2.24-0.833 2.84-0.852 4.9-0.521 6.1-3.77 0.26-0.704 0.404-1.57 0.22-2.31-0.225-0.9-2.44-3.28-3.27-3.7-1.35-0.675-3.05-0.667-4.43-1.01-1.3-0.326-3.08-0.498-4.11 0.524"
            fill="#483f3c"
          />
        </g>
        <rect x="19.1" y="11" width="19.1" height="5.51" fill="#2d2624" stroke-width=".987" />
        <clipPath id="a"><rect x="27.6" y="19.3" width="19.3" height="5.58" /></clipPath>
        <g transform="matrix(.987 0 0 .987 -8.13 -8.03)" clip-path="url(#a)">
          <path
            d="m38.1 18.8c0.144 0.284 0.197 0.749 0.286 1.07 0.466 1.68 0.509 3.53 0.399 5.27-0.041 0.653-0.374 1.31-0.374 1.96 0 0.041 0.076-0.032 0.116-0.043 0.154-0.042 0.14-0.034 0.29-0.06 0.375-0.063 0.754-0.104 1.13-0.153 0.884-0.115 1.77-0.241 2.66-0.34 2.32-0.26 5.58 0.4 6.53-2.44 0.185-0.557 0.236-1.13 0.289-1.72 0.054-0.587 0.14-1.38-0.037-1.95-0.922-3-4.9-1.81-7.22-1.81-0.773 0-1.54 0.084-2.3 0.236-0.055 0.011-0.659 0.108-0.659 0.114"
            fill="#483f3c"
          />
        </g>
        <g fill="#a19e9e" stroke-width=".987">
          <circle cx="16.5" cy="14" r="1.44" />
          <circle cx="40.5" cy="14" r="1.44" />
          <rect x="15.2" y="33.7" width="9.44" height="3.23" />
          <rect x="32.5" y="23.7" width="9.44" height="3.23" />
          <rect x="32.5" y="33.7" width="9.44" height="3.23" />
        </g>
        <g stroke-width=".987">
          <rect x="17.9" y="23.7" width="3.93" height="3.23" fill="#8e7147" />
          <rect x="34.8" y="24.1" width="4.88" height="2.44" fill="#171514" />
          <rect x="34.8" y="34.1" width="4.88" height="2.44" fill="#171514" />
          <text fill="#ffffff" font-size="2.2px" stroke-width=".987">
            <tspan x="35.267719 36.591557 37.915394" y="26.1">103</tspan>
            <tspan x="35.267719 36.591557 37.915394" y="36.12">102</tspan>
          </text>
          <rect x="17.9" y="33.7" width="3.93" height="3.23" fill="#ccf9f9" />
        </g>
      </svg>
    `}};k2=m3([U("wokwi-ir-receiver")],k2);var g3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const y3={power:162,menu:226,test:34,plus:2,back:194,prev:224,play:168,next:144,0:104,minus:152,c:176,1:48,2:24,3:122,4:16,5:56,6:90,7:66,8:74,9:82},x3={o:"power",m:"menu",t:"test","+":"plus",b:"back",arrowleft:"prev",p:"play",arrowright:"next",0:"0","-":"minus",c:"c",1:"1",2:"2",3:"3",4:"4",5:"5",6:"6",7:"7",8:"8",9:"9"};let z2=class extends L{static get styles(){return ke`
      use {
        fill: #fff;
      }

      use.red {
        fill: #e6252e;
      }

      use.black {
        fill: #121115;
      }

      use[tabindex] {
        cursor: pointer;
      }

      use.active {
        fill: #8c8;
      }

      use.red.active,
      use.black.active {
        fill: green;
      }

      use:focus {
        --circle-stroke-color: cyan;
        outline: none;
      }
    `}eventHandler(e,i,s){e.focus();const n=y3[i];switch(s){case"up":e.classList.remove("active"),this.dispatchEvent(new CustomEvent("button-release",{detail:{key:i,irCode:n}}));break;case"down":e.classList.add("active"),this.dispatchEvent(new CustomEvent("button-press",{detail:{key:i,irCode:n}}));break}}buttonEvent(e,i){var s;const n=e.target;if(!(n instanceof SVGElement))return null;const r=(s=n.dataset.btn)!==null&&s!==void 0?s:"";r!=null&&(e.preventDefault(),this.eventHandler(n,r,i))}keyboardEvent(e,i){var s;Te.includes(e.key)&&this.buttonEvent(e,i);const n=e.target,r=x3[e.key.toLowerCase()];if(!(n instanceof SVGElement)||r==null)return;const l=(s=this.shadowRoot)===null||s===void 0?void 0:s.querySelector(`use[data-btn="${r}"]`);l&&l instanceof SVGElement&&this.eventHandler(l,r,i)}render(){return G`
      <?xml version="1.0" encoding="UTF-8"?>
      <svg
        version="1.1"
        viewBox="0 0 151 316"
        width="40mm"
        height="83.653mm"
        font-family="sans-serif"
        xmlns="http://www.w3.org/2000/svg"
        @mousedown=${e=>this.buttonEvent(e,"down")}
        @mouseup=${e=>this.buttonEvent(e,"up")}
        @touchstart=${e=>this.buttonEvent(e,"down")}
        @touchend=${e=>this.buttonEvent(e,"up")}
        @keydown=${e=>this.keyboardEvent(e,"down")}
        @keyup=${e=>this.keyboardEvent(e,"up")}
      >
        <defs>
          <g id="button" stroke-width="1.29">
            <path
              fill="#272726"
              d="m0 -17.5c-9.73 0-17.6 7.9-17.6 17.6 0 9.73 7.9 17.6 17.6 17.6 9.73 0 17.6-7.9 17.6-17.6 0-9.73-7.9-17.6-17.6-17.6zm0 1.29c9.01 0 16.3 7.32 16.3 16.3 0 9.01-7.32 16.3-16.3 16.3-9.02 0-16.3-7.32-16.3-16.3 0-9.02 7.32-16.3 16.3-16.3z"
            />
            <circle r="16.3" style="stroke: var(--circle-stroke-color)" />
          </g>
          <circle id="button2" r="16.3" style="stroke: var(--circle-stroke-color)" />
        </defs>
        <path
          d="m149 21.3c0-10.5-8.52-19-19-19h-109c-10.5 0-19 8.52-19 19v274c0 10.5 8.52 19 19 19h109c10.5 0 19-8.52 19-19z"
          fill="#fff"
          stroke="#272726"
          stroke-width="4.53px"
        />
        <use xlink:href="#button2" x="29.6" y="37.9" data-btn="power" class="red" tabindex="0" />
        <use xlink:href="#button" x="121.4" y="37.9" data-btn="menu" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="29.6" y="75.2" data-btn="test" tabindex="0" fill="#fff" />
        <use xlink:href="#button2" x="75.5" y="75.2" data-btn="plus" class="black" tabindex="0" />
        <use xlink:href="#button" x="121.4" y="75.2" data-btn="back" tabindex="0" fill="#fff" />
        <use xlink:href="#button2" x="29.6" y="113" data-btn="prev" class="black" tabindex="0" />
        <use xlink:href="#button" x="75.5" y="113" data-btn="play" tabindex="0" fill="#fff" />
        <use xlink:href="#button2" x="121.4" y="113" data-btn="next" class="black" tabindex="0" />
        <use xlink:href="#button" x="29.6" y="152" data-btn="0" tabindex="0" fill="#fff" />
        <use xlink:href="#button2" x="75.5" y="152" data-btn="minus" class="black" tabindex="0" />
        <use xlink:href="#button" x="121.4" y="152" data-btn="c" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="29.6" y="190" data-btn="1" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="75.5" y="190" data-btn="2" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="121.4" y="190" data-btn="3" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="29.6" y="228" data-btn="4" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="75.5" y="228" data-btn="5" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="121.4" y="228" data-btn="6" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="29.6" y="266" data-btn="7" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="75.5" y="266" data-btn="8" tabindex="0" fill="#fff" />
        <use xlink:href="#button" x="121.4" y="266" data-btn="9" tabindex="0" fill="#fff" />
        <g style="pointer-events: none">
          <g fill="none" stroke="#fff" stroke-width="1.94px">
            <path
              d="m33.5 33c2.05 1.28 3.42 3.56 3.42 6.16 0 4.01-3.26 7.26-7.26 7.26-4.01 0-7.26-3.25-7.26-7.26 0-2.49 1.26-4.69 3.17-6"
            />
            <path d="m29.6 29.3v7.41" />
          </g>
          <path d="m80.9 113-9.58 4.79v-9.58z" fill="#121115" stroke-width="1.29" />
          <path d="m123.4 113-9.58 4.79v-9.58z" fill="#fff" stroke-width="1.29" />
          <path d="m129.4 113-8.95 4.79v-9.58z" fill="#fff" stroke-width="1.29" />
          <path d="m129.4 109v9.58" fill="none" stroke="#fff" stroke-width="1.29" />
          <path d="m27.9 113 9.58 4.79v-9.58z" fill="#fff" stroke-width="1.29" />
          <path d="m21.8 113 8.95 4.79v-9.58z" fill="#fff" stroke-width="1.29" />
          <path d="m22.4 109v9.58" fill="none" stroke="#fff" stroke-width="1.29" />
          <text fill="#e6252e" font-size="9.72px" font-weight="700" stroke-width="1.29">
            <tspan x="106.892 115.469 122.432 129.931" y="41.288">MENU</tspan>
            <tspan x="16.488 22.904 29.866 36.829" y="78.679">TEST</tspan>
          </text>
          <g fill="none" stroke="#fff" stroke-width="1.29">
            <path d="m67.7 152h15.5" />
            <path d="m67.7 75.2h15.5M75.5 67.4v15.5" />
          </g>
          <g fill="#121115" stroke-width="1.29">
            <path
              d="m119.4 70.7v -3.25l-6.95 4.84 6.71 4.45 0.111-2.2s6.65-0.357 7.05 3.15c0.397 3.51-6.66 5.21-6.66 5.21s10.9-2.33 10.7-6.82c-0.276-5.4-10.9-5.39-10.9-5.39z"
            />
            <text font-size="13.9px" font-weight="700">
              <tspan x="25.312" y="156.434">0</tspan>
              <tspan x="116.973" y="156.498">C</tspan>
              <tspan x="25.312" y="194.685">1</tspan>
              <tspan x="71.776" y="194.685">2</tspan>
              <tspan x="118.06" y="194.6">3</tspan>
              <tspan x="25.312" y="232.851">4</tspan>
              <tspan x="71.776" y="232.679">5</tspan>
              <tspan x="118.199" y="232.767">6</tspan>
              <tspan x="25.312" y="270.931">7</tspan>
              <tspan x="71.776" y="270.931">8</tspan>
              <tspan x="118.124" y="270.931">9</tspan>
            </text>
          </g>
          <g fill="#fff" stroke-width="1.29">
            <path
              d="m18 28.5c0.687-0.757 1.5-1.41 2.39-1.99 1.26-0.814 2.7-1.43 4.22-1.87 0.974-0.281 1.98-0.481 3-0.607 0.673-0.0828 1.35-0.129 2.03-0.147 0.68-0.0181 1.36-0.0078 2.03 0.0427 1.02 0.0789 2.03 0.243 3 0.511 2.48 0.686 4.72 2.02 6.31 4.19 0.0323 0.0479 0.097 0.0608 0.145 0.0298 0.0479-0.0323 0.0621-0.097 0.0298-0.145-0.846-1.45-1.96-2.62-3.27-3.53-0.894-0.623-1.87-1.12-2.91-1.5-1.19-0.433-2.45-0.709-3.73-0.828-0.543-0.0504-1.09-0.0698-1.64-0.0582-0.728 0.0155-1.46 0.0841-2.18 0.202-1.08 0.177-2.14 0.46-3.16 0.839-0.772 0.288-1.51 0.632-2.21 1.03-1.7 0.965-3.16 2.22-4.22 3.7-0.0362 0.0453-0.0298 0.111 0.0155 0.146 0.0453 0.0362 0.11 0.0298 0.146-0.0155z"
            />
            <path
              d="m64 65.5c0.687-0.757 1.5-1.41 2.39-1.99 1.26-0.814 2.7-1.43 4.22-1.87 0.974-0.281 1.98-0.481 3-0.607 0.673-0.0815 1.35-0.129 2.03-0.147 0.679-0.0181 1.36-0.0078 2.03 0.044 1.02 0.0776 2.03 0.242 3 0.51 2.48 0.686 4.72 2.02 6.31 4.19 0.031 0.0479 0.0957 0.0621 0.145 0.0298 0.0479-0.031 0.0608-0.0957 0.0297-0.145-0.847-1.45-1.97-2.62-3.27-3.53-0.892-0.623-1.87-1.12-2.91-1.5-1.19-0.433-2.45-0.709-3.73-0.828-0.545-0.0504-1.09-0.0698-1.64-0.0582-0.728 0.0155-1.46 0.0841-2.18 0.202-1.08 0.177-2.14 0.46-3.16 0.839-0.772 0.288-1.51 0.632-2.22 1.03-1.7 0.965-3.16 2.22-4.22 3.7-0.0362 0.0453-0.0285 0.111 0.0155 0.147 0.0453 0.0362 0.111 0.0285 0.147-0.0168z"
            />
            <path
              d="m18 104c0.687-0.757 1.5-1.42 2.39-1.99 1.26-0.814 2.7-1.43 4.22-1.87 0.974-0.281 1.98-0.481 3-0.607 0.673-0.0815 1.35-0.129 2.03-0.147 0.68-0.0181 1.36-8e-3 2.03 0.044 1.02 0.0776 2.03 0.242 3 0.51 2.48 0.686 4.72 2.02 6.31 4.19 0.0323 0.0478 0.097 0.0621 0.145 0.0297 0.0479-0.031 0.0621-0.0957 0.0298-0.145-0.846-1.45-1.96-2.62-3.27-3.53-0.894-0.623-1.87-1.12-2.91-1.5-1.19-0.433-2.45-0.709-3.73-0.828-0.543-0.0504-1.09-0.0698-1.64-0.0582-0.728 0.0155-1.46 0.0841-2.18 0.202-1.08 0.177-2.14 0.46-3.16 0.839-0.772 0.288-1.51 0.632-2.21 1.03-1.7 0.965-3.16 2.22-4.22 3.7-0.0362 0.0453-0.0298 0.111 0.0155 0.147 0.0453 0.0362 0.11 0.0285 0.146-0.0168z"
            />
            <path
              d="m110.4 104c0.687-0.757 1.5-1.42 2.39-1.99 1.26-0.814 2.7-1.43 4.22-1.87 0.974-0.281 1.98-0.481 3-0.607 0.673-0.0815 1.35-0.129 2.03-0.147 0.68-0.0181 1.36-8e-3 2.03 0.044 1.02 0.0776 2.03 0.242 3 0.51 2.48 0.686 4.72 2.02 6.31 4.19 0.031 0.0478 0.0957 0.0621 0.145 0.0297 0.0479-0.031 0.0608-0.0957 0.0298-0.145-0.847-1.45-1.97-2.62-3.27-3.53-0.892-0.623-1.87-1.12-2.91-1.5-1.19-0.433-2.45-0.709-3.73-0.828-0.545-0.0504-1.09-0.0698-1.64-0.0582-0.728 0.0155-1.46 0.0841-2.18 0.202-1.08 0.177-2.14 0.46-3.16 0.839-0.772 0.288-1.51 0.632-2.22 1.03-1.7 0.965-3.16 2.22-4.22 3.7-0.0362 0.0453-0.0285 0.111 0.0155 0.147 0.0453 0.0362 0.111 0.0285 0.147-0.0168z"
            />
            <path
              d="m64 142c0.687-0.758 1.5-1.42 2.39-1.99 1.26-0.815 2.7-1.43 4.22-1.87 0.974-0.279 1.98-0.481 3-0.605 0.673-0.0828 1.35-0.129 2.03-0.147 0.679-0.0181 1.36-9e-3 2.03 0.0427 1.02 0.0789 2.03 0.243 3 0.511 2.48 0.686 4.72 2.02 6.31 4.19 0.031 0.0491 0.0957 0.0621 0.145 0.031 0.0479-0.0323 0.0608-0.097 0.0297-0.145-0.847-1.45-1.97-2.62-3.27-3.54-0.892-0.623-1.87-1.12-2.91-1.5-1.19-0.435-2.45-0.71-3.73-0.829-0.545-0.0504-1.09-0.0698-1.64-0.0569-0.728 0.0155-1.46 0.0841-2.18 0.202-1.08 0.177-2.14 0.459-3.16 0.838-0.772 0.29-1.51 0.632-2.22 1.03-1.7 0.965-3.16 2.22-4.22 3.7-0.0362 0.044-0.0285 0.11 0.0155 0.146 0.0453 0.0362 0.111 0.0284 0.147-0.0155z"
            />
          </g>
        </g>
      </svg>
    `}};z2=g3([U("wokwi-ir-remote")],z2);var v3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let _2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"VCC",y:92,x:36.178,number:1,signals:[De()]},{name:"OUT",y:92,x:45.9175,number:2,signals:[]},{name:"GND",y:92,x:55.6415,number:3,signals:[$e()]}]}render(){return G`
      <svg
        width="24mm"
        height="24.448mm"
        version="1.1"
        viewBox="0 0 90.7 92.4"
        xmlns="http://www.w3.org/2000/svg"
      >
        <g fill="#c6bf95">
          <path
            d="m54.2 91c0 0.383 0.151 0.749 0.422 1.02 0.27 0.27 0.636 0.422 1.02 0.422h1e-3c0.382 0 0.748-0.152 1.02-0.422s0.422-0.636 0.422-1.02v-26.1c0-0.234-0.189-0.423-0.423-0.423h-2.04c-0.234 0-0.423 0.189-0.423 0.423v26.1z"
          />
          <path
            d="m44.5 91c0 0.383 0.151 0.749 0.422 1.02 0.27 0.27 0.636 0.422 1.02 0.422h1e-3c0.382 0 0.748-0.152 1.02-0.422s0.422-0.636 0.422-1.02v-26.1c0-0.234-0.189-0.423-0.423-0.423h-2.04c-0.234 0-0.423 0.189-0.423 0.423v26.1z"
          />
          <path
            d="m34.7 91c0 0.383 0.152 0.749 0.422 1.02s0.637 0.422 1.02 0.422 0.749-0.152 1.02-0.422 0.422-0.636 0.422-1.02v-26.1c0-0.234-0.19-0.423-0.424-0.423h-2.03c-0.234 0-0.424 0.189-0.424 0.423v26.1z"
          />
        </g>
        <path
          d="m90.7 0h-90.7v74.3h90.7zm-5.38 32.1c2.09 0 3.78 1.7 3.78 3.78s-1.7 3.78-3.78 3.78c-2.09 0-3.78-1.7-3.78-3.78s1.7-3.78 3.78-3.78zm-77.1 0c2.09 0 3.78 1.7 3.78 3.78s-1.7 3.78-3.78 3.78c-2.09 0-3.78-1.7-3.78-3.78s1.7-3.78 3.78-3.78z"
          fill="#253674"
        />
        <rect x="14.3" y="1.28" width="65.5" height="65.5" fill="#dde5e3" />
        <rect x="17.1" y="4.14" width="59.8" height="59.8" fill="#d1dfe1" />
        <circle cx="46.7" cy="33.8" r="31" fill="#d3d5d6" />
        <circle cx="46.7" cy="33.8" r="28.2" fill="#d9e1e1" />
        <clipPath id="a">
          <circle cx="52.5" cy="39.2" r="28.2" />
        </clipPath>
        <g transform="translate(-5.81 -5.45)" clip-path="url(#a)" fill="#fbfcfb">
          <path
            d="m52.8 13.7c11.2 2.94 21.3 18.3 21.8 30.5 0 0 6.16-8.84-2.25-20.6-7.05-9.89-19.5-9.87-19.5-9.87z"
          />
          <path
            d="m52.2 64.8c-7.37 0.013-19.8-6.53-22.1-14 0 0-0.102 4.33 6.99 10.2 5.95 4.94 15.1 3.75 15.1 3.75z"
          />
          <g fill="none" stroke="#d2d8d8" stroke-width=".4px">
            <path d="m32.9 18-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m32.9 27.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m32.9 36.9-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m32.9 46.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m32.9 55.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m32.9 65.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 13.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 22.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 32.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 41.6-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 51-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m41.1 60.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />

            <path d="m49.3 18-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m49.3 27.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m49.3 36.9-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m49.3 46.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m49.3 55.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m49.3 65.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 13.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 22.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 32.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 41.6-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 51-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m57.4 60.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />

            <path d="m65.6 18-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m65.6 27.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m65.6 36.9-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m65.6 46.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m65.6 55.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m65.6 65.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 13.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 22.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 32.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 41.6-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 51-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m73.8 60.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />

            <path d="m81.9 18-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m81.9 27.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m81.9 36.9-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m81.9 46.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m81.9 55.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m81.9 65.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 13.3-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 22.7-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 32.1-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 41.6-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 51-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
            <path d="m90.1 60.4-2.72 4.71h-5.44l-2.72-4.71 2.72-4.71h5.44z" />
          </g>
        </g>
        <text fill="#ffffff" font-family="sans-serif">
          <tspan x="33.293" y="73.864" font-size="9.88px">+</tspan>
          <tspan x="43.531" y="72.609" font-size="6.38px">D</tspan>
        </text>
        <path d="m57.9 70.8h-4.67v-0.81h4.67z" fill="#fff" />
      </svg>
    `}};_2=v3([U("wokwi-pir-motion-sensor")],_2);var w3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let C2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",y:26.2,x:135,number:1,signals:[$e()]},{name:"VCC",y:35.8,x:135,number:2,signals:[De()]},{name:"OUT",y:45.5,x:135,number:3,signals:[H(0)]}]}render(){return G`
      <svg
        width="35.826mm"
        height="19mm"
        version="1.1"
        viewBox="0 0 135.4 71.782"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <clipPath id="a">
            <path
              d="m15.336 49.725c-0.945 0.682-2.127 1.088-3.411 1.088-3.104 0-5.612-2.374-5.612-5.281s2.508-5.281 5.612-5.281c1.038 0 2.009 0.266 2.842 0.728 2.108 0.79 3.314 1.004 5.699 0.917 0 0-2.134 1.335-1.968 2.97 0.149 1.458 3.053 2.494 3.053 2.494-2.438 0.388-4.177 1.403-6.215 2.365z"
            />
          </clipPath>
        </defs>
        <path
          d="m115.3 0h-90.421v71.782h90.421zm-66.145 56.313c3.27 0 5.925 2.608 5.925 5.878s-2.655 5.924-5.925 5.924-5.925-2.654-5.925-5.924 2.655-5.878 5.925-5.878zm16.013-7.96c3.27 0 5.925 2.654 5.925 5.924s-2.655 5.925-5.925 5.925-5.924-2.655-5.924-5.925 2.654-5.924 5.924-5.924zm-33.698 1.324c2.29 0 4.149 1.859 4.149 4.148 0 2.29-1.859 4.149-4.149 4.149-2.289 0-4.148-1.859-4.148-4.149 0-2.289 1.859-4.148 4.148-4.148zm59.914 0.635c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm-11.4-8.143c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm-14.816-1.811c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm0-15.974c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm14.816-3.203c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm-14.816-9.601c3.27 0 5.925 2.654 5.925 5.924s-2.655 5.925-5.925 5.925-5.924-2.655-5.924-5.925 2.654-5.924 5.924-5.924zm-33.698 2.228c2.29 0 4.149 1.859 4.149 4.148 0 2.29-1.859 4.149-4.149 4.149-2.289 0-4.148-1.859-4.148-4.149 0-2.289 1.859-4.148 4.148-4.148zm59.914 0.288c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698zm-48.154-5.701c0-1.635 2.963-4.729 5.925-4.729s5.925 3.094 5.925 4.729c0 3.27-2.655 7.121-5.925 7.121s-5.925-3.851-5.925-7.121z"
          fill="#0f3661"
        />
        <path
          d="m104.45 21.602v28.578h8.389v-28.578z"
          fill="none"
          stroke="#fff"
          stroke-width=".9px"
        />
        <g fill="#29261c">
          <path d="m105.37 42.328v6.554h6.554v-6.554z" />
          <path d="m105.37 32.604v6.554h6.554v-6.554z" />
          <path d="m105.37 22.865v6.554h6.554v-6.554z" />
        </g>
        <g fill="#9f9f9f">
          <path
            d="m108.85 44.165c-0.382 0-0.749 0.151-1.019 0.422-0.27 0.27-0.422 0.636-0.422 1.018v1e-3c0 0.382 0.152 0.748 0.422 1.018s0.637 0.422 1.019 0.422h26.131c0.234 0 0.424-0.189 0.424-0.423v-2.035c0-0.234-0.19-0.423-0.424-0.423h-26.131z"
          />
          <path
            d="m108.85 34.441c-0.382 0-0.749 0.151-1.019 0.422-0.27 0.27-0.422 0.636-0.422 1.018v1e-3c0 0.382 0.152 0.748 0.422 1.018s0.637 0.422 1.019 0.422h26.131c0.234 0 0.424-0.189 0.424-0.423v-2.035c0-0.234-0.19-0.423-0.424-0.423h-26.131z"
          />
          <path
            d="m108.85 24.701c-0.382 0-0.749 0.152-1.019 0.422-0.27 0.271-0.422 0.637-0.422 1.019s0.152 0.749 0.422 1.019 0.637 0.422 1.019 0.422h26.131c0.234 0 0.424-0.19 0.424-0.423v-2.035c0-0.234-0.19-0.424-0.424-0.424h-26.131z"
          />
        </g>
        <path d="m96.494 43.126v-14.495h-4.787v14.495z" fill="#bbb9b9" />
        <path d="m96.661 39.537v-7.317h-5.121v7.317z" fill="#29261c" />
        <g fill="none" stroke="#bbb9b9" stroke-linejoin="miter">
          <circle cx="31.465" cy="17.956" r="4.149" stroke-width="2.5px" />
          <circle cx="31.465" cy="53.825" r="4.149" stroke-width="2.5px" />
          <circle cx="65.163" cy="54.277" r="5.925" stroke-width=".95px" />
          <circle cx="65.163" cy="17.504" r="5.925" stroke-width=".95px" />
          <circle cx="65.163" cy="28.082" r="3.698" stroke-width="2.23px" />
          <circle cx="65.163" cy="44.056" r="3.698" stroke-width="2.23px" />
          <circle cx="49.15" cy="62.191" r="5.925" stroke-width=".75px" />
          <circle cx="49.15" cy="9.591" r="5.925" stroke-width=".75px" />
        </g>
        <ellipse cx="48.82" cy="25.397" rx="6.375" ry="4.839" fill="#bababa" />
        <ellipse cx="48.82" cy="46.384" rx="6.375" ry="4.839" fill="#bbb9b9" />
        <circle cx="48.82" cy="25.397" r="2.612" fill="#eceee9" />
        <circle cx="48.82" cy="46.384" r="2.612" fill="#eceee9" />
        <path
          d="m48.82 25.397c-8.828 4.288-19.813 9.008-38 11.393"
          fill="none"
          stroke="#d6d8d4"
          stroke-linejoin="miter"
          stroke-width=".95px"
        />
        <path
          d="m48.82 45.922c-9.482-5.223-20.452-6.013-38-4.789"
          fill="none"
          stroke="#d8d8d3"
          stroke-linejoin="miter"
          stroke-width=".95px"
        />
        <path
          d="m9.023 43.72c-0.945 0.682-2.127 1.088-3.411 1.088-3.104 0-5.612-2.374-5.612-5.281s2.508-5.281 5.612-5.281c1.038 0 2.009 0.266 2.842 0.728 2.108 0.79 3.314 1.004 5.699 0.917 0 0-2.134 1.335-1.968 2.97 0.149 1.458 3.053 2.494 3.053 2.494-2.438 0.388-4.177 1.403-6.215 2.365z"
          fill="#151312"
        />
        <g transform="translate(-6.313,-6.005)" clip-path="url(#a)">
          <path
            d="m16.648 41.782c-0.617 0-1.284-0.077-1.895 0-2.276 0.284-4.755 1.806-6.429 3.282-0.732 0.645-1.351 1.332-1.854 2.171-0.172 0.287-0.363 0.562-0.527 0.852-8e-3 0.012-0.215 0.396-0.248 0.362-0.152-0.151-0.044-0.995-0.044-1.151 0-1.394 0.015-2.694 0.341-4.059 0.435-1.827 0.867-4.205 2.407-5.497 0.593-0.497 1.419-0.714 2.138-0.941 0.989-0.311 2.096-0.55 3.145-0.406 1.754 0.241 3.113 2.109 3.428 3.768 0.08 0.421-0.08 0.892-0.08 1.31"
            fill="#615a59"
          />
        </g>
        <g r="3.698" fill="none" stroke="#bbb9b9" stroke-linejoin="miter" stroke-width="2.23px">
          <circle cx="91.379" cy="17.794" />
          <circle cx="91.379" cy="54.01" />
        </g>

        <path
          d="m79.979 41.028c3.519 0 6.375 2.168 6.375 4.839 0 2.67-2.856 4.839-6.375 4.839-3.518 0-6.375-2.169-6.375-4.839 0-2.671 2.857-4.839 6.375-4.839zm0 1.141c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698z"
          fill="#bbb9b9"
        />
        <path
          d="m79.979 20.04c3.519 0 6.375 2.169 6.375 4.839 0 2.671-2.856 4.839-6.375 4.839-3.518 0-6.375-2.168-6.375-4.839 0-2.67 2.857-4.839 6.375-4.839zm0 1.141c2.041 0 3.698 1.657 3.698 3.698s-1.657 3.698-3.698 3.698-3.698-1.657-3.698-3.698 1.657-3.698 3.698-3.698z"
          fill="#bbb9b9"
        />
        <path
          d="m89.905 44.462v-17.142h8.391v17.142z"
          fill="none"
          stroke="#fff"
          stroke-linejoin="miter"
          stroke-width=".65px"
        />
        <text fill="#fffefe" font-family="sans-serif" transform="rotate(-90)">
          <tspan x="-39.297 -37.036 -34.776" y="95.418" font-size="3.735px">103</tspan>
          <tspan x="-61.485" y="111.57" font-size="9.778px">S</tspan>
          <tspan x="-15.512" y="111.573" font-size="15.828px">-</tspan>
        </text>
      </svg>
    `}};C2=w3([U("wokwi-ntc-temperature-sensor")],C2);var b3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let $2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",y:17.8,x:87,number:1,signals:[$e()]},{name:"VCC",y:27.5,x:87,number:2,signals:[De()]},{name:"OUT",y:37.5,x:87,number:3,signals:[]}]}render(){return G`
      <svg
        width="23.4mm"
        height="20.943mm"
        version="1.1"
        viewBox="0 0 88.4 79.2"
        xmlns="http://www.w3.org/2000/svg"
      >
        <path
          d="m71.2 0h-71.2v55.6h71.2zm-62.6 41.4c2.65 0 4.79 2.15 4.79 4.79 0 2.64-2.15 4.79-4.79 4.79-2.64 0-4.79-2.15-4.79-4.79 0-2.65 2.15-4.79 4.79-4.79zm0-36.7c2.65 0 4.79 2.15 4.79 4.79 0 2.64-2.15 4.79-4.79 4.79-2.64 0-4.79-2.15-4.79-4.79 0-2.65 2.15-4.79 4.79-4.79z"
          fill="#19365e"
        />
        <g transform="rotate(-90 31 151)">
          <text
            x="132.20599"
            y="184.995"
            fill="#fffefe"
            font-family="sans-serif"
            font-size="10.3px"
          >
            s
          </text>
        </g>
        <circle cx="22.6" cy="46.9" r="3.23" fill="#bbb9b9" />
        <circle cx="33.4" cy="46.9" r="3.23" fill="#bbb9b9" />
        <path d="m57.5 13.5v28.6h8.39v-28.6z" fill="none" stroke="#fff" stroke-width=".9px" />
        <g fill="#29261c">
          <path d="m58.4 34.2v6.55h6.55v-6.55z" />
          <path d="m58.4 24.5v6.55h6.55v-6.55z" />
          <path d="m58.4 14.8v6.56h6.55v-6.56z" />
        </g>
        <g fill="#9f9f9f">
          <path
            d="m61.9 36.1c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.637-0.422 1.02 0.152 0.748 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.04c0-0.233-0.189-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m61.9 26.3c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.637-0.422 1.02 0.152 0.748 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.04c0-0.233-0.189-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m61.9 16.6c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.636-0.422 1.02v1e-3c0 0.382 0.152 0.748 0.422 1.02s0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.189 0.423-0.423v-2.04c0-0.234-0.189-0.423-0.423-0.423h-26.1z"
          />
        </g>
        <g
          transform="translate(-6.88 -4.2)"
          fill="#0e0f0d"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width="1.83px"
        >
          <circle cx="29.8" cy="22.6" r="2.59" />
          <circle cx="29.8" cy="12.2" r="2.59" />
          <circle cx="29.8" cy="41.3" r="2.59" />
          <circle cx="39.9" cy="22.6" r="2.59" />
          <circle cx="39.9" cy="12.2" r="2.59" />
          <circle cx="39.9" cy="41.3" r="2.59" />
        </g>
        <circle
          cx="8.58"
          cy="9.42"
          r="4.79"
          fill="none"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width="1.1px"
        />
        <circle
          cx="8.58"
          cy="46.2"
          r="4.79"
          fill="none"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width="1.1px"
        />
        <g transform="translate(-6.88 -4.2)">
          <rect x="26.5" y="59.8" width="16.4" height="20.9" fill="#d3d9de" />
          <circle cx="34.8" cy="64.3" r="2.37" fill="#a8b2c8" />
          <path
            d="m40.7 62.8h-2.75v19.2c0 0.364 0.145 0.713 0.403 0.971 0.257 0.258 0.607 0.402 0.971 0.402h1e-3c0.364 0 0.714-0.144 0.971-0.402 0.258-0.258 0.403-0.607 0.403-0.971v-19.2z"
            fill="#b9c5de"
          />
          <rect x="37.9" y="62.8" width="2.75" height="17.9" fill="#a8b2c8" />
          <path
            d="m32.4 69.5h-2.75v12.5c0 0.364 0.145 0.713 0.402 0.971 0.258 0.258 0.607 0.402 0.972 0.402s0.714-0.144 0.972-0.402c0.257-0.258 0.402-0.607 0.402-0.971v-12.5z"
            fill="#b9c5de"
          />
          <g fill="#a8b2c8">
            <rect x="29.6" y="69.5" width="2.75" height="11.2" />
            <path
              d="m35.5 72.2c0.142 0 0.277-0.056 0.377-0.156 0.101-0.1 0.157-0.236 0.157-0.377v-1.68c0-0.142-0.056-0.277-0.157-0.377-0.1-0.1-0.235-0.157-0.377-0.157h-3.97c-0.364 0-0.714 0.145-0.971 0.403-0.258 0.257-0.403 0.607-0.403 0.971v1e-3c0 0.364 0.145 0.713 0.403 0.971 0.257 0.258 0.607 0.402 0.971 0.402h3.97z"
            />
            <path
              d="m38.8 65.5c0.141 0 0.277-0.056 0.377-0.156s0.157-0.236 0.157-0.377v-1.68c0-0.142-0.057-0.277-0.157-0.377-0.1-0.101-0.236-0.157-0.377-0.157h-3.97c-0.364 0-0.714 0.145-0.972 0.403-0.257 0.257-0.402 0.607-0.402 0.971v1e-3c0 0.364 0.145 0.713 0.402 0.971 0.258 0.258 0.608 0.402 0.972 0.402h3.97z"
            />
          </g>
        </g>
        <path
          d="m31.8 15h2.49v-6.79c0-0.33-0.131-0.647-0.365-0.88-0.233-0.234-0.55-0.365-0.88-0.365h-1e-3c-0.33 0-0.647 0.131-0.88 0.365-0.234 0.233-0.365 0.55-0.365 0.88z"
          fill="#d2d2d2"
        />
        <path
          d="m21.7 15h2.49v-6.79c0-0.33-0.131-0.647-0.365-0.88-0.233-0.234-0.55-0.365-0.88-0.365h-1e-3c-0.33 0-0.647 0.131-0.88 0.365-0.234 0.233-0.365 0.55-0.365 0.88z"
          fill="#d2d2d2"
        />
        <g transform="translate(-6.88 -4.2)">
          <rect x="47" y="29.2" width="13.4" height="4.43" fill="#bbb9b9" />
          <rect x="50.3" y="29" width="6.77" height="4.74" fill="#29261c" />
        </g>
        <g transform="translate(-6.88 -4.2)">
          <rect x="47" y="20" width="13.4" height="4.43" fill="#bbb9b9" />
          <rect x="50.3" y="19.9" width="6.77" height="4.74" fill="#29261c" />
        </g>
        <path
          d="m38.9 23.3h15.9v7.76h-15.9z"
          fill="none"
          stroke="#fff"
          stroke-linejoin="miter"
          stroke-width=".6px"
        />
        <path
          d="m38.9 14.1h15.9v7.76h-15.9z"
          fill="none"
          stroke="#fff"
          stroke-linejoin="miter"
          stroke-width=".6px"
        />
        <path
          d="m37.4 15.4h-19v19.1c0 5.24 4.24 9.48 9.48 9.48 5.24 0 9.48-4.24 9.48-9.48z"
          fill="#fdfefe"
        />
        <clipPath id="c">
          <path d="m44.3 19.6h-19v19.1c0 5.24 4.24 9.48 9.48 9.48 5.24 0 9.48-4.24 9.48-9.48z" />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#c)">
          <path
            d="m30.6 18.1c0 8.12-1.48 16.2-0.263 24.3 0.388 2.58 1.14 4.94 2.59 7.11 0.478 0.718 0.956 1.5 1.51 2.16 0.201 0.236 0.416 0.375 0.672 0.529 0.102 0.061 0.438 0.157 0.319 0.157-3.1 0-5.53-2.5-7.49-4.64-4.21-4.59-5.36-8.3-5.88-14.5-0.078-0.921-0.402-1.9-0.353-2.81 0.073-1.36 0.578-2.79 0.921-4.11 0.564-2.16 1.08-4.18 2.51-5.92 0.417-0.508 0.545-1.27 1.08-1.69 0.624-0.494 2.43-0.722 3.1-0.28 0.189 0.124 0.829 0.279 0.829 0.56"
            fill="#fff"
          />
        </g>
        <path
          d="m35.4 15.4h-15v18.8c0 1.98 0.789 3.89 2.19 5.29 1.4 1.4 3.31 2.19 5.29 2.19s3.89-0.788 5.29-2.19c1.4-1.4 2.19-3.31 2.19-5.29z"
          fill="#d5d5d5"
        />
        <clipPath id="b">
          <path
            d="m42.3 19.6h-15v18.8c0 1.98 0.789 3.89 2.19 5.29 1.4 1.4 3.31 2.19 5.29 2.19s3.89-0.788 5.29-2.19c1.4-1.4 2.19-3.31 2.19-5.29z"
          />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#b)">
          <g fill="#b1b1b1">
            <path d="m31.4 29.6v-10h-3.02v13.7h12.9l-3.67-3.65z" />
            <path d="m41.3 30.4-3.18-3.2v-7.56h3.18z" />
          </g>
          <path
            d="m30.6 18.1c0 8.12-1.48 16.2-0.263 24.3 0.388 2.58 1.14 4.94 2.59 7.11 0.478 0.718 0.956 1.5 1.51 2.16 0.201 0.236 0.416 0.375 0.672 0.529 0.102 0.061 0.438 0.157 0.319 0.157-3.1 0-5.53-2.5-7.49-4.64-4.21-4.59-5.36-8.3-5.88-14.5-0.078-0.921-0.402-1.9-0.353-2.81 0.073-1.36 0.578-2.79 0.921-4.11 0.564-2.16 1.08-4.18 2.51-5.92 0.417-0.508 0.545-1.27 1.08-1.69 0.624-0.494 2.43-0.722 3.1-0.28 0.189 0.124 0.829 0.279 0.829 0.56"
            fill="#e2e2e2"
          />
          <clipPath id="a">
            <path
              d="m30.6 18.1c0 8.12-1.48 16.2-0.263 24.3 0.388 2.58 1.14 4.94 2.59 7.11 0.478 0.718 0.956 1.5 1.51 2.16 0.201 0.236 0.416 0.375 0.672 0.529 0.102 0.061 0.438 0.157 0.319 0.157-3.1 0-5.53-2.5-7.49-4.64-4.21-4.59-5.36-8.3-5.88-14.5-0.078-0.921-0.402-1.9-0.353-2.81 0.073-1.36 0.578-2.79 0.921-4.11 0.564-2.16 1.08-4.18 2.51-5.92 0.417-0.508 0.545-1.27 1.08-1.69 0.624-0.494 2.43-0.722 3.1-0.28 0.189 0.124 0.829 0.279 0.829 0.56"
            />
          </clipPath>
          <g clip-path="url(#a)">
            <path d="m31.4 29.6v-10h-3.02v13.7h12.9l-3.67-3.65z" fill="#c7c7c7" />
          </g>
        </g>
        <rect x="17.3" y="11.7" width="21.3" height="3.68" fill="#fdfefe" />
        <path
          d="m64 9.39h-4.68"
          fill="none"
          stroke="#fffefe"
          stroke-linejoin="miter"
          stroke-width=".85px"
        />
      </svg>
    `}};$2=b3([U("wokwi-heart-beat-sensor")],$2);var k3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let E2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"GND",y:18,x:88,number:1,signals:[$e()]},{name:"VCC",y:27.8,x:88,number:2,signals:[De()]},{name:"OUT",y:37.5,x:88,number:3,signals:[]}]}render(){return G`
      <svg
        width="23.4mm"
        height="14.7mm"
        version="1.1"
        viewBox="0 0 88.4 55.6"
        font-family="sans-serif"
        xmlns="http://www.w3.org/2000/svg"
      >
        <path
          d="m71.2 0h-71.2v55.6h71.2zm-54.1 43.8c2.66 0 4.82 2.22 4.82 4.96s-2.16 4.96-4.82 4.96-4.82-2.22-4.82-4.96 2.16-4.96 4.82-4.96zm13.8-3.78c2.03 0 3.68 1.7 3.68 3.78s-1.65 3.78-3.68 3.78-3.68-1.7-3.68-3.78 1.65-3.78 3.68-3.78zm0-31.5c2.03 0 3.68 1.7 3.68 3.78 0 2.09-1.65 3.78-3.68 3.78s-3.68-1.7-3.68-3.78c0-2.09 1.65-3.78 3.68-3.78zm-13.8-6.6c2.66 0 4.82 2.22 4.82 4.96s-2.16 4.96-4.82 4.96-4.82-2.22-4.82-4.96 2.16-4.96 4.82-4.96z"
          fill="#19365e"
        />
        <clipPath id="g">
          <path
            d="m78.1 4.2h-71.2v55.6h71.2zm-54.1 43.8c2.66 0 4.82 2.22 4.82 4.96s-2.16 4.96-4.82 4.96-4.82-2.22-4.82-4.96 2.16-4.96 4.82-4.96zm13.8-3.78c2.03 0 3.68 1.7 3.68 3.78s-1.65 3.78-3.68 3.78-3.68-1.7-3.68-3.78 1.65-3.78 3.68-3.78zm0-31.5c2.03 0 3.68 1.7 3.68 3.78 0 2.09-1.65 3.78-3.68 3.78s-3.68-1.7-3.68-3.78c0-2.09 1.65-3.78 3.68-3.78zm-13.8-6.6c2.66 0 4.82 2.22 4.82 4.96s-2.16 4.96-4.82 4.96-4.82-2.22-4.82-4.96 2.16-4.96 4.82-4.96z"
          />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#g)">
          <path
            d="m49.8 12.7c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.255 0.299 0.705 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.255 0.334-0.705 0.079-1-1.83-2.14-8.55-9.99-10.4-12.1z"
            fill="#1b2f4c"
          />
        </g>
        <text
          transform="rotate(-90 31 151)"
          x="132.20"
          y="184.995"
          fill="#fffefe"
          font-size="10.3px"
        >
          s
        </text>
        <g fill="#bbb9b9">
          <ellipse cx="18.5" cy="18.5" rx="5.21" ry="3.44" />
          <ellipse cx="42.3" cy="18.5" rx="5.21" ry="3.44" />
          <ellipse cx="18.5" cy="37.2" rx="5.21" ry="3.44" />
          <ellipse cx="42.3" cy="37.2" rx="5.21" ry="3.44" />
        </g>
        <path d="m57.5 13.5v28.6h8.39v-28.6z" fill="none" stroke="#fff" stroke-width=".9px" />
        <g fill="#29261c">
          <path d="m58.4 34.2v6.55h6.55v-6.55z" />
          <path d="m58.4 24.5v6.55h6.55v-6.55z" />
          <path d="m58.4 14.8v6.56h6.55v-6.56z" />
        </g>
        <g fill="#9f9f9f">
          <path
            d="m61.9 36.1c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.637-0.422 1.02 0.152 0.748 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.04c0-0.233-0.189-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m61.9 26.3c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.637-0.422 1.02 0.152 0.748 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.04c0-0.233-0.189-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m61.9 16.6c-0.382 0-0.748 0.152-1.02 0.422s-0.422 0.636-0.422 1.02v1e-3c0 0.382 0.152 0.748 0.422 1.02s0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.189 0.423-0.423v-2.04c0-0.234-0.189-0.423-0.423-0.423h-26.1z"
          />
        </g>
        <g fill="#0e0f0d" stroke="#bbb9b9" stroke-linejoin="miter" stroke-width="1.83px" r="2.59">
          <circle cx="42.2" cy="18.4" />
          <circle cx="18.4" cy="18.4" />
          <circle cx="18.4" cy="37.1" />
          <circle cx="42.2" cy="37.1" />
        </g>
        <ellipse
          cx="17.1"
          cy="48.7"
          rx="4.82"
          ry="4.96"
          fill="none"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width="1.12px"
        />
        <path
          d="m30.8 39.6c-2.26 0-4.1 1.88-4.1 4.21 0 2.33 1.84 4.21 4.1 4.21s4.1-1.88 4.1-4.21c0-2.33-1.84-4.21-4.1-4.21zm0 0.855c1.8 0 3.25 1.51 3.25 3.36s-1.45 3.36-3.25 3.36-3.25-1.51-3.25-3.36 1.45-3.36 3.25-3.36z"
          fill="#bbb9b9"
        />
        <clipPath id="f">
          <path
            d="m37.7 43.8c-2.26 0-4.1 1.88-4.1 4.21 0 2.33 1.84 4.21 4.1 4.21s4.1-1.88 4.1-4.21c0-2.33-1.84-4.21-4.1-4.21zm0 0.855c1.8 0 3.25 1.51 3.25 3.36s-1.45 3.36-3.25 3.36-3.25-1.51-3.25-3.36 1.45-3.36 3.25-3.36z"
          />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#f)">
          <path
            d="m49.8 12.7c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.255 0.299 0.705 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.255 0.334-0.705 0.079-1-1.83-2.14-8.55-9.99-10.4-12.1z"
            fill="#8e8e8e"
          />
        </g>
        <ellipse
          cx="30.8"
          cy="12.3"
          rx="3.68"
          ry="3.78"
          fill="none"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width=".86px"
        />
        <ellipse
          cx="17.1"
          cy="6.86"
          rx="4.82"
          ry="4.96"
          fill="none"
          stroke="#bbb9b9"
          stroke-linejoin="miter"
          stroke-width="1.12px"
        />
        <path
          d="m24.6 33.6-1.62-1.89s-3.45 2.95-5.16 4.41c-0.251 0.215-0.407 0.521-0.432 0.85-0.026 0.329 0.08 0.655 0.295 0.906v1e-3c0.215 0.25 0.521 0.406 0.85 0.431 0.329 0.026 0.655-0.08 0.906-0.295 1.72-1.47 5.16-4.42 5.16-4.42z"
          fill="#d2d2d2"
        />
        <clipPath id="e">
          <path
            d="m31.5 37.8-1.62-1.89s-3.45 2.95-5.16 4.41c-0.251 0.215-0.407 0.521-0.432 0.85-0.026 0.329 0.08 0.655 0.295 0.906v1e-3c0.215 0.25 0.521 0.406 0.85 0.431 0.329 0.026 0.655-0.08 0.906-0.295 1.72-1.47 5.16-4.42 5.16-4.42z"
          />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#e)">
          <path
            d="m31 36.9c-0.189 0.025-0.301 0.185-0.449 0.294-0.418 0.309-0.808 0.635-1.19 0.985-1 0.927-2.03 1.82-3.05 2.73-0.259 0.231-0.576 0.388-0.836 0.617-0.587 0.517-1.13 1.14-1.91 1.35-0.089 0.025-0.25 0.124-0.334 0.124-0.057 0 0.112-6e-3 0.168 0 0.093 9e-3 0.179 0.032 0.275 0.037 0.342 0.017 0.701 0.016 1.04-5e-3 0.994-0.062 2.05-0.419 3.01-0.675 1.54-0.411 2.9-1.01 4.09-2.12 0.52-0.486 0.938-1.08 1.38-1.64 0.582-0.723 1.1-1.47 1.41-2.35 0.018-0.052-0.326-0.106-0.365-0.114-0.472-0.095-0.911-0.123-1.4-0.123-0.655 0-1.44 0.626-2 0.894-0.412 0.196-1.25 0.443-1.56 0.866"
            fill="#ececec"
          />
        </g>
        <g transform="translate(-6.88 -4.2)">
          <path d="m56.6 33.8v10.9h3.6v-10.9z" fill="#bbb9b9" />
          <path d="m56.5 36.5v5.5h3.85v-5.5z" fill="#29261c" />
        </g>
        <g transform="translate(-6.88 -4.2)">
          <path d="m56.6 19.7v10.9h3.6v-10.9z" fill="#bbb9b9" />
          <clipPath id="d">
            <path d="m56.6 19.7v10.9h3.6v-10.9z" />
          </clipPath>
          <g clip-path="url(#d)">
            <path
              d="m49.8 12.7c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.255 0.299 0.705 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.255 0.334-0.705 0.079-1-1.83-2.14-8.55-9.99-10.4-12.1z"
              fill="#9c9b9b"
            />
          </g>
          <path d="m56.5 22.4v5.5h3.85v-5.5z" fill="#29261c" />
          <clipPath id="c">
            <path d="m56.5 22.4v5.5h3.85v-5.5z" />
          </clipPath>
          <g clip-path="url(#c)">
            <path
              d="m49.8 12.7c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.255 0.299 0.705 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.255 0.334-0.705 0.079-1-1.83-2.14-8.55-9.99-10.4-12.1z"
            />
          </g>
        </g>
        <path
          d="m54.7 28.6v12.9h-6.3v-12.9z"
          fill="none"
          stroke="#fff"
          stroke-linejoin="miter"
          stroke-width=".49px"
        />
        <path d="m48.2 14.3v13.4h6.79v-13.4zm0.489 5.11v-4.62h5.81v4.62 7.76h-5.81z" fill="#fff" />
        <clipPath id="b">
          <path d="m55 18.5v13.4h6.79v-13.4zm0.489 5.11v-4.62h5.81v4.62 7.76h-5.81z" />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#b)">
          <path
            d="m49.8 12.7c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.255 0.299 0.705 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.255 0.334-0.705 0.079-1-1.83-2.14-8.55-9.99-10.4-12.1z"
            fill="#d4d4d4"
          />
        </g>
        <path
          d="m61.6 10.4v-4.68"
          fill="none"
          stroke="#fffefe"
          stroke-linejoin="miter"
          stroke-width=".85px"
        />
        <g fill="#a19e9e">
          <circle cx="4.64" cy="41.4" r="2.88" />
          <circle cx="4.64" cy="14.2" r="2.88" />
          <circle cx="51.4" cy="8.74" r="2.88" />
          <circle cx="51.4" cy="45.6" r="2.88" />
        </g>
        <g fill="#29261c">
          <circle cx="51.4" cy="45.6" r="1.47" />
          <circle cx="4.64" cy="41.4" r="1.47" />
          <circle cx="4.64" cy="14.2" r="1.47" />
          <circle cx="51.4" cy="8.74" r="1.47" />
        </g>
        <path
          d="m45.4 12.3 1.65 1.86s3.4-3 5.09-4.5c0.247-0.219 0.397-0.527 0.417-0.857 0.02-0.329-0.091-0.653-0.31-0.901h-1e-3c-0.218-0.247-0.527-0.398-0.856-0.418-0.33-0.02-0.654 0.092-0.901 0.311-1.69 1.5-5.09 4.5-5.09 4.5z"
          fill="#d2d2d2"
        />
        <clipPath id="a">
          <path
            d="m52.2 16.5 1.65 1.86s3.4-3 5.09-4.5c0.247-0.219 0.397-0.527 0.417-0.857 0.02-0.329-0.091-0.653-0.31-0.901h-1e-3c-0.218-0.247-0.527-0.398-0.856-0.418-0.33-0.02-0.654 0.092-0.901 0.311-1.69 1.5-5.09 4.5-5.09 4.5z"
          />
        </clipPath>
        <g transform="translate(-6.88 -4.2)" clip-path="url(#a)">
          <path
            d="m52.8 17.4c0.188-0.028 0.297-0.19 0.444-0.302 0.413-0.316 0.797-0.648 1.17-1 0.986-0.943 2-1.86 3-2.78 0.255-0.235 0.569-0.398 0.825-0.63 0.579-0.527 1.11-1.16 1.89-1.38 0.088-0.026 0.248-0.127 0.332-0.129 0.057-1e-3 -0.112 8e-3 -0.168 3e-3 -0.093-7e-3 -0.18-0.03-0.276-0.033-0.342-0.011-0.701-4e-3 -1.04 0.023-0.993 0.079-2.04 0.453-2.99 0.725-1.53 0.437-2.89 1.06-4.05 2.19-0.512 0.494-0.92 1.1-1.36 1.66-0.569 0.733-1.08 1.49-1.37 2.37-0.017 0.053 0.328 0.101 0.367 0.108 0.474 0.088 0.913 0.107 1.4 0.099 0.654-0.011 1.42-0.649 1.98-0.927"
            fill="#ececec"
          />
        </g>
        <path
          d="m42.2 6.32c-0.256-0.299-0.705-0.334-1-0.078-3.3 2.82-20.7 17.7-24 20.5-0.299 0.255-0.334 0.705-0.078 1 1.83 2.14 8.55 9.99 10.4 12.1 0.256 0.299 0.706 0.334 1 0.078 3.3-2.82 20.7-17.7 24-20.5 0.299-0.256 0.334-0.705 0.078-1-1.83-2.14-8.55-9.99-10.4-12.1z"
          fill="#4a91ce"
        />
      </svg>
    `}};E2=k3([U("wokwi-tilt-switch")],E2);var en=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let Y1=class extends L{constructor(){super(...arguments);this.ledPower=!1,this.ledSignal=!1,this.pinInfo=[{name:"VCC",y:14.6,x:199,number:1,signals:[De()]},{name:"GND",y:24.3,x:199,number:2,signals:[$e()]},{name:"DOUT",y:34,x:199,number:3,signals:[]},{name:"AOUT",y:43.7,x:199,number:4,signals:[]}]}render(){const{ledPower:e,ledSignal:i}=this;return G`
      <svg
        width="52.904mm"
        heigth="16.267mm"
        version="1.1"
        viewBox="0 0 200 61.5"
        xmlns="http://www.w3.org/2000/svg"
      >
        <path
          d="m180 1.49e-7h-136v61.5h136zm-129 52c1.9 0 3.44 1.5 3.44 3.34s-1.54 3.34-3.44 3.34-3.44-1.5-3.44-3.34 1.54-3.34 3.44-3.34zm98.3-29.8c4.17 0 7.55 3.38 7.55 7.55 0 4.17-3.38 7.55-7.55 7.55s-7.55-3.38-7.55-7.55c0-4.17 3.38-7.55 7.55-7.55zm-98.3-19.4c1.9 0 3.44 1.5 3.44 3.34 0 1.84-1.54 3.34-3.44 3.34s-3.44-1.5-3.44-3.34c0-1.84 1.54-3.34 3.44-3.34z"
          fill="#1c2546"
        />
        <rect
          x="45.5"
          y="20.1"
          width="11.2"
          height="22.2"
          fill="none"
          stroke="#fff"
          stroke-width="1.08px"
        />
        <circle cx="51.1" cy="25.6" r="3.14" fill="#dae3eb" />
        <circle cx="51.1" cy="36.8" r="3.14" fill="#dae3eb" />
        <path
          d="m51.1 25.7c0-0.199-0.079-0.39-0.219-0.53-0.141-0.141-0.332-0.22-0.53-0.22h-13.2v1.5h13.2c0.198 0 0.389-0.079 0.53-0.219 0.14-0.141 0.219-0.332 0.219-0.53z"
          fill="#a8b6ba"
        />
        <path
          d="m51.1 36.7c0-0.198-0.079-0.389-0.219-0.53-0.141-0.14-0.332-0.219-0.53-0.219h-13.2v1.5h13.2c0.198 0 0.389-0.079 0.53-0.22 0.14-0.14 0.219-0.331 0.219-0.53z"
          fill="#a8b6ba"
        />
        <path
          d="m35.2 20.1h-25.2c-5.49 0-9.94 4.45-9.94 9.94v1e-3c0 5.49 4.45 9.94 9.94 9.94h25.2z"
          fill="#29261c"
        />
        <clipPath id="c">
          <path
            d="m35.18 20.14h-25.2c-5.49 0-9.94 4.45-9.94 9.94v1e-3c0 5.49 4.45 9.94 9.94 9.94h25.2z"
          />
        </clipPath>
        <g clip-path="url(#c)">
          <path
            d="m37.68 21.94c-12.6 0-25.1-0.227-37.7 0.917-0.196 0.018 0 4.25 0 4.25 12.8-1.41 25-1.08 37.7-0.669z"
            fill="#47434d"
          />
        </g>
        <rect x="32.6" y="17.9" width="5.06" height="25.7" fill="#29261c" />
        <clipPath id="b">
          <rect x="32.68" y="17.94" width="5.06" height="25.7" />
        </clipPath>
        <g clip-path="url(#b)">
          <path
            d="m51.98 20.84c-12.6 0-25.1-0.228-37.7 0.916-0.195 0.018 0 4.25 0 4.25 12.8-1.41 25-1.08 37.7-0.669z"
            fill="#47434d"
          />
        </g>
        <g fill="none" stroke-width="1.08px">
          <g stroke="#fff">
            <path d="m70.2 32.4h-7.47v9.67h7.47" />
            <path d="m136 10.4v-7.47h-9.67v7.47" />
            <path d="m155 12.8h7.47v-9.67h-7.47" />
            <path d="m155 55.8h7.47v-9.67h-7.47" />
            <path d="m136 43v-7.47h-9.67v7.47" />
            <path d="m70.2 45.2h-7.47v9.67h7.47" />
            <path d="m75.1 32.4h7.47v9.67h-7.47" />
            <path d="m136 15.2v7.47h-9.67v-7.47" />
            <path d="m151 12.8h-7.47v-9.67h7.47" />
            <path d="m151 55.8h-7.47v-9.67h7.47" />
            <path d="m136 47.9v7.47h-9.67v-7.47" />
            <path d="m75.1 45.2h7.47v9.67h-7.47" />
            <path
              d="m75.1 20h4.46c2.67 0 4.83 2.16 4.83 4.84v1e-3c0 2.67-2.16 4.84-4.83 4.84h-4.46"
            />
            <path
              d="m75.1 6.62h4.46c2.67 0 4.83 2.16 4.83 4.83v1e-3c0 2.67-2.16 4.84-4.83 4.84h-4.46"
            />
            <path
              d="m70.2 20h-4.46c-2.67 0-4.83 2.16-4.83 4.84v1e-3c0 2.67 2.16 4.84 4.83 4.84h4.46"
            />
            <path
              d="m70.2 6.62h-4.46c-2.67 0-4.83 2.16-4.83 4.83v1e-3c0 2.67 2.16 4.84 4.83 4.84h4.46"
            />
          </g>
          <ellipse cx="51.1" cy="6.11" rx="3.43" ry="3.34" stroke="#a8b6ba" />
          <ellipse cx="51.1" cy="55.4" rx="3.43" ry="3.34" stroke="#a8b6ba" />
          <g stroke="#fff">
            <path d="m50.6 44.7v4.75" />
            <path d="m50.6 12.1v4.75" />
            <path d="m53 14.5h-4.75" />
          </g>
        </g>
        <g fill="#dae3eb">
          <rect x="64.3" y="8.69" width="16.7" height="5.52" />
          <rect x="64.3" y="22" width="16.7" height="5.52" />
          <rect x="64.3" y="34.5" width="16.7" height="5.52" />
        </g>
        <rect x="68.4" y="34.3" width="8.43" height="5.9" fill="#29261c" />
        <path d="m134 21.2v-16.7h-5.52v16.7z" fill="#dae3eb" />
        <path d="m134 17v-8.43h-5.9v8.43z" fill="#29261c" />

        <!-- LEDs -->
        <rect x="145" y="5.23" width="16.7" height="5.52" fill="#dae3eb" />
        <rect x="149" y="5.04" width="8.43" height="5.9" fill="#fffefe" />
        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="2" />
        </filter>
        ${e&&V`<circle cx="153.2" cy="7.5" r="7" fill="#80ff80" filter="url(#ledFilter)" />`}
        <rect x="145" y="48.3" width="16.7" height="5.52" fill="#dae3eb" />
        <rect x="149" y="48.1" width="8.43" height="5.9" fill="#fffefe" />
        ${i&&V`<circle cx="153.2" cy="51.3" r="7" fill="#80ff80" filter="url(#ledFilter)" />`}

        <path d="m134 53.8v-16.7h-5.52v16.7z" fill="#dae3eb" />
        <path d="m134 49.7v-8.43h-5.9v8.43z" fill="#29261c" />
        <rect x="64.3" y="47.5" width="16.7" height="5.52" fill="#dae3eb" />
        <rect x="68.4" y="8.5" width="8.43" height="5.9" fill="#907463" />
        <rect x="68.4" y="21.8" width="8.43" height="5.9" fill="#907463" />
        <rect x="68.4" y="47.3" width="8.43" height="5.9" fill="#29261c" />
        <g fill="#dae3eb">
          <path
            d="m99.2 34.6h-9.67c-0.406 0-0.796 0.162-1.08 0.449-0.288 0.287-0.449 0.677-0.449 1.08v1e-3c0 0.406 0.161 0.796 0.449 1.08 0.287 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m99.2 40.4h-9.67c-0.406 0-0.796 0.162-1.08 0.449-0.288 0.287-0.449 0.677-0.449 1.08v1e-3c0 0.406 0.161 0.796 0.449 1.08 0.287 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m99.2 46.2h-9.67c-0.406 0-0.796 0.162-1.08 0.449-0.288 0.287-0.449 0.677-0.449 1.08v1e-3c0 0.406 0.161 0.796 0.449 1.08 0.287 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m99.2 52h-9.67c-0.406 0-0.796 0.162-1.08 0.449-0.288 0.287-0.449 0.677-0.449 1.08v1e-3c0 0.406 0.161 0.796 0.449 1.08 0.287 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m111 55.1h9.67c0.406 0 0.796-0.161 1.08-0.448 0.287-0.288 0.448-0.678 0.448-1.08v-1e-3c0-0.406-0.161-0.796-0.448-1.08-0.288-0.287-0.678-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m111 49.3h9.67c0.406 0 0.796-0.161 1.08-0.448 0.287-0.288 0.448-0.678 0.448-1.08v-1e-3c0-0.406-0.161-0.796-0.448-1.08-0.288-0.287-0.678-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m111 43.5h9.67c0.406 0 0.796-0.161 1.08-0.448 0.287-0.288 0.448-0.678 0.448-1.08v-1e-3c0-0.406-0.161-0.796-0.448-1.08-0.288-0.287-0.678-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m111 37.7h9.67c0.406 0 0.796-0.161 1.08-0.448 0.287-0.288 0.448-0.678 0.448-1.08v-1e-3c0-0.406-0.161-0.796-0.448-1.08-0.288-0.287-0.678-0.449-1.08-0.449h-9.67z"
          />
        </g>
        <rect x="97.6" y="33.2" width="16.1" height="23.3" fill="#29261c" />
        <rect x="89.4" width="32.1" height="32.1" fill="#466fb5" />
        <g fill="none" stroke="#2e60aa" stroke-width="1.63px">
          <path d="m95.6 7.35 13.9 17.1" />
          <path d="m112 8.62-14.9 13.2" />
          <path d="m114 12.7-19.2 5.13" />
          <path d="m114 17.3-19.5-4.06" />
          <path d="m113 21.8-5.14-4.58" />
          <path d="m105 27.6v-9.83" />
          <path d="m100 24.7 3.14-5.18" />
        </g>
        <circle cx="105" cy="16" r="6.56" fill="#bcc2d5" />
        <clipPath id="a">
          <circle cx="105.08" cy="16.04" r="6.56" />
        </clipPath>
        <g clip-path="url(#a)" fill="none" stroke="#3f3c40" stroke-width="2.5px">
          <path d="m105.08 6.74v18.6" />
          <path d="m115.08 16.04h-18.6" />
        </g>
        <path
          d="m149 19.8c5.5 0 9.96 4.46 9.96 9.96s-4.46 9.96-9.96 9.96-9.96-4.46-9.96-9.96 4.46-9.96 9.96-9.96zm0 2.4c4.17 0 7.55 3.38 7.55 7.55 0 4.17-3.38 7.55-7.55 7.55s-7.55-3.38-7.55-7.55c0-4.17 3.38-7.55 7.55-7.55z"
          fill="#d4d0d1"
        />
        <path
          d="m169.1 10.2v38.6h8.39v-38.6z"
          fill="#1c2546"
          stroke="#fff"
          stroke-linejoin="round"
          stroke-width=".9px"
        />
        <g fill="#29261c">
          <path d="m170 40.75v6.55h6.55v-6.55z" />
          <path d="m170 31.05v6.55h6.55v-6.55z" />
          <path d="m170 21.25v6.55h6.55v-6.55z" />
          <path d="m170 11.55v6.55h6.55v-6.55z" />
        </g>
        <g fill="#9f9f9f">
          <path
            d="m173 42.55c-0.382 0-0.748 0.152-1.02 0.422-0.271 0.271-0.422 0.637-0.422 1.02s0.151 0.749 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.423v-2.04c0-0.234-0.189-0.424-0.423-0.424h-26.1z"
          />
          <path
            d="m173 32.85c-0.382 0-0.748 0.152-1.02 0.422-0.271 0.271-0.422 0.637-0.422 1.02s0.151 0.749 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.423v-2.04c0-0.234-0.189-0.424-0.423-0.424h-26.1z"
          />
          <path
            d="m173 23.15c-0.382 0-0.748 0.152-1.02 0.422-0.271 0.27-0.422 0.637-0.422 1.02s0.151 0.749 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.03c0-0.234-0.189-0.424-0.423-0.424h-26.1z"
          />

          <path
            d="m173 13.45c-0.382 0-0.748 0.152-1.02 0.422-0.271 0.27-0.422 0.637-0.422 1.02s0.151 0.749 0.422 1.02c0.27 0.27 0.636 0.422 1.02 0.422h26.1c0.234 0 0.423-0.19 0.423-0.424v-2.03c0-0.234-0.189-0.424-0.423-0.424h-26.1z"
          />
        </g>
        <text fill="#fffefe" font-family="sans-serif" font-size="8px">
          <tspan x="171" y="10">+</tspan>
          <tspan x="164.3" y="26.5">-</tspan>
          <tspan x="162.5" y="36.9">D</tspan>
          <tspan x="171" y="56">A</tspan>
        </text>
      </svg>
    `}};en([y()],Y1.prototype,"ledPower",void 0);en([y()],Y1.prototype,"ledSignal",void 0);Y1=en([U("wokwi-flame-sensor")],Y1);var z3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let S2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"AOUT",y:16.5,x:137,number:1,signals:[]},{name:"DOUT",y:26.4,x:137,number:2,signals:[]},{name:"GND",y:36.5,x:137,number:3,signals:[$e()]},{name:"VCC",y:46.2,x:137,number:4,signals:[De()]}]}render(){return G`
      <svg
        width="36.232mm"
        height="16.617mm"
        fill-rule="evenodd"
        version="1.1"
        viewBox="0 0 137 59.5"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="a" width="4.1" height="4.1" patternUnits="userSpaceOnUse">
            <path
              d="m0 0v4.09h0.4v-0.85l0.42 0.381v0.469h0.4v-0.0996l0.109 0.0996h0.711v-0.799l0.42 0.379v0.42h0.398v-0.0488l0.0547 0.0488h0.766v-0.75l0.42 0.381v0.369h0.4v-4.09h-0.4v0.311l-0.334-0.311h-0.598l0.111 0.0996v0.9l-0.42-0.379v-0.621h-0.398v0.25l-0.277-0.25h-0.6l0.0566 0.0508v0.9l-0.42-0.381v-0.57h-0.4v0.201l-0.223-0.201zm0.4 0.359 0.42 0.381v0.9l-0.42-0.381zm1.64 0.0508 0.42 0.391v0.889l-0.42-0.379zm1.64 0.0605 0.42 0.379v0.891l-0.42-0.381zm-2.46 0.639 0.42 0.381v0.9l-0.42-0.381zm1.64 0.0508 0.42 0.381v0.898l-0.42-0.379zm-2.46 0.641 0.42 0.379v0.9l-0.42-0.379zm1.64 0.0488 0.42 0.381v0.9l-0.42-0.381zm1.64 0.0508 0.42 0.379v0.9l-0.42-0.379zm-2.46 0.65 0.42 0.379v0.9l-0.42-0.379zm1.64 0.0488 0.42 0.381v0.9l-0.42-0.381z"
              fill="#949392"
            />
          </pattern>
          <g id="pin">
            <path
              fill="#c6bf95"
              d="m29 4.6c0.382 0 0.748-0.152 1.02-0.422s0.422-0.636 0.422-1.02v-1e-3c0-0.382-0.152-0.748-0.422-1.02s-0.636-0.422-1.02-0.422h-26.1c-0.234 0-0.423 0.189-0.423 0.423v2.04c0 0.234 0.189 0.423 0.423 0.423h26.1z"
            />
            <rect x="0" y="0" width="6.9" height="6.9" />
          </g>
        </defs>

        <!-- Board -->
        <path
          d="m113 0h-113v59.5h113zm-1.6 53.2c0 2.62-2.12 4.74-4.74 4.74s-4.74-2.12-4.74-4.74c0-2.62 2.12-4.74 4.74-4.74s4.74 2.12 4.74 4.74zm-110 0c0 2.62 2.12 4.74 4.74 4.74 2.62 0 4.74-2.12 4.74-4.74 0-2.62-2.12-4.74-4.74-4.74-2.62 0-4.74 2.12-4.74 4.74zm105-51.6c2.62 0 4.74 2.12 4.74 4.74 0 2.62-2.12 4.74-4.74 4.74s-4.74-2.12-4.74-4.74c0-2.62 2.12-4.74 4.74-4.74zm-101 0c-2.62 0-4.74 2.12-4.74 4.74 0 2.62 2.12 4.74 4.74 4.74 2.62 0 4.74-2.12 4.74-4.74 0-2.62-2.12-4.74-4.74-4.74z"
          fill="#0664af"
        />

        <!-- Pins -->
        <use xlink:href="#pin" x="107" y="12" />
        <use xlink:href="#pin" x="107" y="21.3" />
        <use xlink:href="#pin" x="107" y="31.1" />
        <use xlink:href="#pin" x="107" y="40.9" />

        <!-- Sensor -->
        <circle cx="47.7" cy="29.8" r="31.2" fill="none" stroke="#fff" stroke-width=".4px" />
        <circle cx="47.7" cy="29.8" r="28.8" fill="#dedede" />
        <circle cx="47.7" cy="29.8" r="25.8" fill="#d0ccc4" />
        <circle cx="47.7" cy="29.8" r="21.4" fill="#bab3ad" />
        <circle cx="47.7" cy="29.8" r="21.4" fill="url(#a)" />

        <text fill="#ffffff" font-family="sans-serif" font-size="3.72px">
          <tspan x="94.656" y="16.729">AOUT</tspan>
          <tspan x="94.656" y="26.098">DOUT</tspan>
          <tspan x="94.656" y="35.911">GND</tspan>
          <tspan x="94.656" y="45.696">VCC</tspan>
        </text>
      </svg>
    `}};S2=z3([U("wokwi-gas-sensor")],S2);var k1=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let Ct=class extends L{constructor(){super(...arguments);this.led1=!1,this.ledPower=!1,this.resetPressed=!1,this.pinInfo=[{name:"GND.1",x:218.5,y:23.3,signals:[{type:"power",signal:"GND"}]},{name:"VCC.1",x:218.5,y:32.9,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"PB4",x:218.5,y:42.5,signals:[H(2),{type:"pwm"}]},{name:"PB5",x:218.5,y:52.2,signals:[H(0)]},{name:"PB3",x:218.5,y:61.7,signals:[H(3)]},{name:"PB2",x:218.5,y:71.2,signals:[te("SCK"),ve("SCL"),H(1)]},{name:"PB1",x:218.5,y:80.9,signals:[te("MISO"),{type:"pwm"}]},{name:"PB0",x:218.5,y:90.5,signals:[te("MOSI"),ve("SDA"),{type:"pwm"}]},{name:"VIN",x:132.7,y:8.1,signals:[{type:"power",signal:"VCC"}]},{name:"GND.2",x:142.9,y:8.1,signals:[{type:"power",signal:"GND"}]},{name:"VCC.2",x:153,y:8.1,signals:[{type:"power",signal:"VCC",voltage:5}]}]}static get styles(){return ke`
      text {
        font-size: 2px;
        font-family: monospace;
        user-select: none;
      }
      circle[tabindex]:hover,
      circle[tabindex]:focus {
        stroke: #eee;
        outline: none;
      }
    `}render(){const{ledPower:e,led1:i}=this;return G`
      <svg
        width="64mm"
        height="30mm"
        version="1.1"
        viewBox="0 0 64 30"
        xmlns="http://www.w3.org/2000/svg"
      >
        <filter id="ledFilter" x="-0.8" y="-0.8" height="2.8" width="2.8">
          <feGaussianBlur stdDeviation="0.5" />
        </filter>

        ${vi}

        <!-- PCB -->
        <path
          d="M63.1 0H12.5v30h50.6V0zM22.2 25.9a1.57 1.57 0 11-.002 3.142A1.57 1.57 0 0122.2 25.9zm38.4 0a1.57 1.57 0 11-.002 3.138A1.57 1.57 0 0160.6 25.9zM22.2 1a1.57 1.57 0 110 3.14 1.57 1.57 0 010-3.14zm38.4 0a1.57 1.57 0 11-.002 3.142A1.57 1.57 0 0160.6 1z"
          fill="#2c8240"
        />

        <!-- Holes -->
        <g fill="#e6e6e6">
          <path
            d="M30.6 3.3a.752.752 0 10-1.503 0v1.46a.752.752 0 001.503 0V3.3zM26.3 3.3a.752.752 0 00-1.504 0v1.46a.752.752 0 001.504 0V3.3z"
          />
          <path
            d="M48.2 6.44a.748.748 0 00-.748-.748h-1.41a.748.748 0 000 1.496h1.41a.748.748 0 00.748-.748zM54.2 6.44a.748.748 0 00-.748-.748h-1.41a.748.748 0 000 1.496h1.41a.748.748 0 00.748-.748zM54.2 2.09a.748.748 0 00-.748-.748h-1.41a.748.748 0 000 1.496h1.41a.748.748 0 00.748-.748zM48.2 2.09a.748.748 0 00-.748-.748h-1.41a.748.748 0 000 1.496h1.41a.748.748 0 00.748-.748z"
          />
        </g>

        <g fill="#848400">
          <circle cx="15.4" cy="20.8" r="1.57" />
          <circle cx="15.4" cy="9.28" r="1.57" />
          <circle cx="29.2" cy="7.69" r=".94" />
          <circle cx="21.7" cy="7.69" r=".94" />
          <circle cx="21.7" cy="10.5" r=".94" />
          <circle cx="29.2" cy="10.5" r=".94" />
          <circle cx="36.7" cy="10.6" r=".94" />
          <circle cx="33.6" cy="10.6" r=".94" />
          <circle cx="36.7" cy="20.5" r=".94" />
          <circle cx="33.6" cy="20.5" r=".94" />
          <circle cx="20.6" cy="22.7" r=".94" />
          <circle cx="21.8" cy="19.8" r=".94" />
          <circle cx="29.3" cy="19.8" r=".94" />
          <circle cx="30.6" cy="16.9" r=".94" />
          <circle cx="20.6" cy="16.9" r=".94" />
          <circle cx="30.6" cy="13.9" r=".94" />
          <circle cx="17.8" cy="11.7" r=".94" />
          <circle cx="17.8" cy="13.9" r=".94" />
          <circle cx="20.6" cy="13.9" r=".94" />
          <circle cx="17.8" cy="16.1" r=".94" />
          <circle cx="17.8" cy="18.3" r=".94" />
          <circle cx="30.6" cy="22.7" r=".94" />
          <circle cx="33.6" cy="27" r=".94" />
          <circle cx="33.6" cy="24.8" r=".94" />
          <circle cx="38.4" cy="27" r=".94" />
          <circle cx="38.4" cy="24.8" r=".94" />
        </g>

        <g fill="#fff">
          <circle cx="15.4" cy="20.8" r="1.05" />
          <circle cx="15.4" cy="9.28" r="1.05" />
          <circle cx="29.2" cy="7.69" r=".52" />
          <circle cx="21.7" cy="7.69" r=".52" />
          <circle cx="21.7" cy="10.5" r=".52" />
          <circle cx="29.2" cy="10.5" r=".52" />
          <circle cx="36.7" cy="10.6" r=".52" />
          <circle cx="33.6" cy="10.6" r=".52" />
          <circle cx="36.7" cy="20.5" r=".52" />
          <circle cx="33.6" cy="20.5" r=".52" />
          <circle cx="20.6" cy="22.7" r=".52" />
          <circle cx="21.8" cy="19.8" r=".52" />
          <circle cx="29.3" cy="19.8" r=".52" />
          <circle cx="30.6" cy="16.9" r=".52" />
          <circle cx="20.6" cy="16.9" r=".52" />
          <circle cx="30.6" cy="13.9" r=".52" />
          <circle cx="17.8" cy="11.7" r=".52" />
          <circle cx="17.8" cy="13.9" r=".52" />
          <circle cx="20.6" cy="13.9" r=".52" />
          <circle cx="17.8" cy="16.1" r=".52" />
          <circle cx="17.8" cy="18.3" r=".52" />
          <circle cx="30.6" cy="22.7" r=".52" />
          <circle cx="33.6" cy="27" r=".52" />
          <circle cx="33.6" cy="24.8" r=".52" />
          <circle cx="38.4" cy="27" r=".52" />
          <circle cx="38.4" cy="24.8" r=".52" />
        </g>

        <!-- USB Connector -->
        <g fill="#b2b2b2">
          <path d="M15.6 11h1.02v1.53H15.6zM16.6 11.5h1.38v.45H16.6z" />
          <path d="M15.6 13.2h1.02v1.53H15.6zM16.6 13.7h1.38v.45H16.6z" />
          <path d="M15.6 15.3h1.02v1.53H15.6zM16.6 15.9h1.38v.45H16.6z" />
          <path d="M15.6 17.5h1.02v1.53H15.6zM16.6 18.1h1.38v.45H16.6z" />
        </g>

        <path d="M-.145 9.97h15.8v10.1h-15.8z" fill="#999" />
        <path d="M-.147 9.97h15v9.24h-15z" fill="#ccc" />
        <path
          d="M11.4 14.6h.682v.877H11.4zM2.45 16.3h1.41v2.08H2.45zM2.45 11.1h1.41v2.08H2.45z"
          fill="#999"
        />

        <!-- Pin Headers -->
        <g transform="translate(59.1 4.7) rotate(90 0 0)">
          <rect width="${.38+2.54*8}" height="2.54" fill="url(#pins-female)" />
        </g>

        <!-- Pin Labels -->
        <text fill="#fff">
          <tspan x="59.5" dy="6.54">GND</tspan>
          <tspan x="59.5" dy="2.54">VCC</tspan>
          <tspan x="60.5" dy="2.54">5</tspan>
          <tspan x="60.5" dy="2.54">4</tspan>
          <tspan x="60.5" dy="2.54">3</tspan>
          <tspan x="60.5" dy="2.54">2</tspan>
          <tspan x="60.5" dy="2.54">1</tspan>
          <tspan x="60.5" dy="2.54">0</tspan>
        </text>

        <!-- Pin Bar -->
        <path d="M41.8 1.37l-.588-.588h-1.53l-.587.588v1.53l.587.587h1.53l.588-.587V2.22z" />
        <path d="M39.2 1.37l-.588-.587h-1.53l-.588.587v1.53l.588.587h1.53l.588-.587V2.22z" />
        <path d="M34.4 3.48h1.53l.588-.587v-1.53l-.588-.587H34.4l-.587.587v1.53l.587.587z" />

        <g fill="#8c8663">
          <path d="M40.1 1.7h.863v.864H40.1z" />
          <path d="M37.4 1.7h.863v.864H37.4z" />
          <path d="M34.7 1.7h.863v.864H34.7z" />
        </g>

        <text transform="translate(33.1 6) rotate(270 0 0)" fill="#fff" style="font-size: 1.4">
          <tspan x="0" dy="2.5">VIN</tspan>
          <tspan x="0" dy="2.5">GND</tspan>
          <tspan x="0" dy="2.5">VCC</tspan>
        </text>

        <!-- MCU -->
        <path
          d="M52.6 17.7h1.22v.873H52.6zM50.2 17.7h1.22v.873H50.2zM47.8 17.7h1.22v.873H47.8zM45.3 17.7h1.22v.873H45.3zM45.3 10.6h1.22v.858H45.3zM47.8 10.6h1.22v.858H47.8zM50.2 10.6h1.22v.858H50.2zM52.6 10.6h1.22v.858H52.6z"
          fill="#b2b2b2"
        />
        <path d="M44.7 11.5h9.77v6.28H44.7z" fill="#313131" />

        <g fill="#212121">
          <path d="M54.4 15.7a1.13 1.13 0 010-2.26v2.27z" />
          <circle cx="53.3" cy="12.6" r=".52" />
        </g>

        <text x="45.14" y="13.4" fill="olive" style="font-family: sans-serif; font-size: 1.6">
          <tspan x="45.14" y="13.4">ATTINY85</tspan>
        </text>

        <!-- LED Silk -->
        <path
          d="M34.6 27.65a2.003 2.003 0 001.01-1.74m.001 0a2 2 0 00-.711-1.53m-3.31 1.53c0 .741.41 1.42 1.06 1.77m-.341-3.32a2.01 2.01 0 00-.723 1.54m7.79 1.74a2.003 2.003 0 001.01-1.74m-.001 0a2 2 0 00-.711-1.53m-3.31 1.53c0 .741.41 1.42 1.06 1.77m-.342-3.32a2.008 2.008 0 00-.723 1.54"
          fill="none"
          stroke="#fff"
          stroke-width=".25"
        />

        <!-- Power LED -->
        <g fill="#00d300">
          <circle cx="33.6" cy="25.9" r="1.6" fill-opacity=".64" />
          <circle cx="33.6" cy="25.9" r="1.2" fill-opacity=".92" />
        </g>
        ${e&&V`<circle cx="33.6" cy="25.9" r="1.8" fill="#03f704" filter="url(#ledFilter)" />`}

        <!-- LED1 -->
        <g fill="#d8e208">
          <circle cx="38.35" cy="25.9" r="1.6" fill-opacity=".64" />
          <circle cx="38.35" cy="25.9" r="1.2" fill-opacity=".92" />
        </g>
        ${i&&V`<circle cx="38.35" cy="25.9" r="1.8" fill="#fcfd00" filter="url(#ledFilter)" />`}

        <g fill="#fff">
          <text x="32.5" dy="23.4">ON</text>
          <text x="36.3" dy="23.4">LED1</text>
        </g>

        <!-- Capacitors -->
        <circle cx="41" cy="9.6" r="2.7" />
        <circle cx="41" cy="9.6" r="1.74" fill="#b2b2b2" />
        <path
          d="M41.1 11.3c-.429.003-.664-.089-1.1-.35l-.288 1.01c.396.226.842.349 1.3.356a2.84 2.84 0 001.4-.375l-.256-1.02c-.425.26-.555.371-1.06.375z"
          fill="#fff"
        />

        <ellipse cx="34.7" cy="7.91" rx="2.68" ry="1.13" fill="#f60" />
        <ellipse cx="34.7" cy="7.5" rx="1.25" ry=".413" />

        <!-- Diodes -->
        <g fill="#b2b2b2">
          <path
            d="M29.55 7.69a.357.357 0 00-.356-.357h-7.5a.356.356 0 000 .713h7.5c.196 0 .356-.16.356-.356z"
          />
          <path
            d="M29.55 10.5a.357.357 0 00-.356-.357h-7.5a.356.356 0 000 .713h7.5c.196 0 .356-.16.356-.356z"
          />
          <path
            d="M29.65 19.8a.357.357 0 00-.356-.356h-7.5a.357.357 0 100 .713h7.5c.196 0 .356-.16.356-.357z"
          />
        </g>

        <g fill="#ff2a2a">
          <path d="M23.2 8.88h4.47c.135 0 .111-2.38 0-2.38H23.2c-.145 0-.157 2.38 0 2.38z" />
          <path d="M27.7 11.7h-4.47c-.135 0-.112-2.38 0-2.38h4.47c.145 0 .157 2.38 0 2.38z" />
          <path d="M27.7 21h-4.47c-.134 0-.111-2.38 0-2.38h4.47c.145 0 .158 2.38 0 2.38z" />
        </g>

        <g fill="#fff" fill-opacity=".74">
          <path d="M24.1 9.32h.563v2.38H24.1z" />
          <path d="M24 18.6h.563v2.38H24z" />
          <path d="M26.4 6.5h.563v2.38H26.4z" />
        </g>

        <g fill-opacity=".4">
          <path
            d="M23.2 6.5c.205-.005.406 0 .609 0h3.86c.026 0 .057.24.052.24h-4.59c-.006 0 .025-.239.066-.24z"
          />
          <path
            d="M27.7 9.32c-.206-.005-.407 0-.609 0h-3.86c-.026 0-.057.239-.052.239h4.59c.006 0-.024-.238-.065-.239z"
          />
          <path
            d="M27.7 18.6c-.205-.005-.406 0-.609 0h-3.86c-.026 0-.057.239-.052.239h4.59c.006 0-.025-.238-.066-.239z"
          />
        </g>

        <!-- Resistors -->
        <g fill="#b2b2b2">
          <path
            d="M31 13.9a.356.356 0 00-.356-.356h-10.1a.356.356 0 100 .712h10.1A.356.356 0 0031 13.9z"
          />
          <path
            d="M31 16.9a.356.356 0 00-.356-.356h-10.1a.356.356 0 100 .713h10.1c.197 0 .356-.16.356-.357z"
          />
          <path
            d="M31 22.7a.356.356 0 00-.356-.356h-10.1a.356.356 0 100 .713h10.1c.197 0 .356-.16.356-.357z"
          />
          <path
            d="M33.6 10.2a.356.356 0 00-.356.356v9.98a.356.356 0 00.713 0v-9.98a.356.356 0 00-.357-.356z"
          />
          <path
            d="M36.7 10.2a.356.356 0 00-.357.356v9.98a.356.356 0 00.713 0v-9.98a.356.356 0 00-.356-.356z"
          />
        </g>

        <g fill="#d9b477">
          <path
            d="M23.8 15.8a1.369 1.369 0 00-.477-.103h-.69a.253.253 0 00-.25.25v1.86c0 .138.113.25.25.25h.69c.138 0 .352-.047.477-.102l.025-.013c.152-.062.314-.097.478-.102h2.49c.138 0 .352.047.477.102l.026.013c.124.057.339.102.477.102h.689c.138 0 .25-.112.251-.25v-1.86a.252.252 0 00-.251-.25h-.689a1.42 1.42 0 00-.477.103l-.026.012a1.369 1.369 0 01-.477.103h-2.49c-.138 0-.353-.048-.478-.103L23.8 15.8z"
          />
          <path
            d="M23.7 12.8a1.351 1.351 0 00-.477-.103h-.69a.252.252 0 00-.25.25v1.86c0 .138.112.25.25.25h.69c.137 0 .352-.047.477-.102l.025-.012c.125-.058.34-.103.477-.103h2.49c.138 0 .353.048.478.103l.025.012c.125.057.34.102.477.102h.69c.137 0 .25-.113.25-.25v-1.86a.252.252 0 00-.25-.25h-.69a1.42 1.42 0 00-.477.103l-.025.012a1.357 1.357 0 01-.478.103h-2.49a1.42 1.42 0 01-.477-.103L23.7 12.8z"
          />
          <path
            d="M23.8 21.6a1.373 1.373 0 00-.477-.102h-.69a.25.25 0 00-.25.25v1.86c0 .138.112.25.25.25h.69c.137 0 .352-.048.477-.103l.025-.012c.125-.057.34-.102.477-.102h2.49c.138 0 .353.047.477.102l.026.012c.125.058.339.103.477.103h.69c.137 0 .25-.113.25-.25v-1.86a.251.251 0 00-.25-.25h-.69c-.138 0-.352.047-.477.102l-.026.013a1.368 1.368 0 01-.477.102h-2.49a1.42 1.42 0 01-.477-.102L23.8 21.6z"
          />
          <path
            d="M34.7 13.8c.058-.125.103-.34.103-.477v-.69a.252.252 0 00-.25-.25h-1.86a.25.25 0 00-.25.25v.69c0 .137.047.352.102.477l.013.025c.057.125.102.34.102.477v2.49c0 .138-.047.353-.102.477l-.013.026a1.373 1.373 0 00-.102.477v.69c0 .138.112.25.25.25h1.86a.252.252 0 00.25-.25v-.69c0-.138-.048-.352-.103-.477l-.012-.026a1.369 1.369 0 01-.103-.477v-2.49c0-.137.048-.352.103-.477l.012-.025z"
          />
          <path
            d="M37.7 13.7c.058-.124.103-.339.103-.477v-.69a.253.253 0 00-.25-.25h-1.86a.252.252 0 00-.25.25v.69c0 .138.047.353.102.477l.012.026c.058.124.103.339.103.477v2.49c0 .138-.047.352-.103.477l-.012.026a1.346 1.346 0 00-.102.477v.69c0 .138.112.25.25.25h1.86a.253.253 0 00.25-.25v-.69a1.41 1.41 0 00-.103-.477l-.012-.026a1.369 1.369 0 01-.103-.477v-2.49c0-.138.048-.353.103-.477l.012-.026z"
          />
        </g>

        <path d="M24.6 21.71h.642v1.92H24.6z" fill="#008000" />

        <g fill="#00f">
          <path
            d="M23.9 12.85a.809.809 0 01-.107-.04l-.025-.013a1.378 1.378 0 00-.478-.102h-.03v2.36h.03c.138 0 .353-.047.478-.103l.025-.012a.688.688 0 01.107-.04V12.9z"
          />
          <path
            d="M23.9 15.85a.822.822 0 01-.108-.04l-.025-.013a1.373 1.373 0 00-.477-.102h-.03v2.36h.03c.137 0 .352-.047.477-.102l.025-.013a.7.7 0 01.108-.04V15.9z"
          />
        </g>

        <g fill="#8a3d06">
          <path
            d="M24 21.66a.696.696 0 01-.108-.039l-.025-.013a1.397 1.397 0 00-.477-.103h-.03v2.36h.03c.137 0 .352-.048.477-.103l.025-.012a.583.583 0 01.108-.04V21.7z"
          />
          <path
            d="M34.64 13.9c.013-.04.025-.077.04-.107l.013-.026c.057-.124.102-.339.102-.477v-.03h-2.36v.03c0 .138.047.353.103.477l.012.026c.015.03.028.067.04.107h2.05z"
          />
          <path
            d="M37.65 13.9a.822.822 0 01.04-.108l.013-.025c.057-.125.102-.34.102-.477v-.03h-2.36v.03c0 .137.047.352.102.477l.012.025c.016.03.028.068.04.108h2.05z"
          />
        </g>

        <g fill="#ad9f4e">
          <path d="M27.8 12.7h.244v2.36H27.8z" />
          <path d="M27.9 15.7h.244v2.36H27.9z" />
          <path d="M35.5 17.8h2.36v.244H35.5z" />
          <path d="M27.8 21.5h.244v2.36H27.8z" />
          <path d="M32.4 17.8h2.36v.244H32.4z" />
        </g>

        <g fill="#b3b3b3">
          <path d="M24.5 12.9h.642v1.92H24.5z" />
          <path d="M24.5 15.9h.642v1.92H24.5z" />
        </g>

        <g fill="#c40808">
          <path d="M25.9 21.72h.642v1.92H25.9z" />
          <path d="M32.65 15.8h1.92v.642H32.6z" />
          <path d="M35.65 15.8h1.92v.642H35.7z" />
        </g>

        <g fill="#1a1a1a">
          <path d="M25.8 12.9h.642v1.92H25.8z" />
          <path d="M25.8 15.9h.642v1.92H25.8z" />
          <path d="M32.65 14.5h1.92v.642H32.6z" />
          <path d="M35.65 14.5h1.92v.642H35.7z" />
        </g>

        <!-- Regulator -->
        <ellipse cx="27.7" cy="2.93" rx="2.31" ry="2.3" fill="#1a1a1a" />
        <path d="M25.4 2.93h4.61v2.3H25.4z" />

        <!-- Reset Button  -->
        <path
          d="M46.1 6.1h1.17v.69H46.1zM52.2 6.1h1.17v.69H52.2zM52.2 1.74h1.17v.69H52.2zM46.1 1.74h1.17v.69H46.1z"
          fill="#b2b2b2"
        />
        <path
          d="M52.5 2.16a.535.535 0 00-.534-.535h-4.49a.535.535 0 00-.534.535v4.28c0 .295.239.535.534.535h4.49c.295 0 .534-.24.534-.535V2.16z"
          fill="#999"
        />

        <circle cx="47.8" cy="6.18" r=".4" />
        <circle cx="51.7" cy="6.18" r=".4" />
        <circle cx="51.7" cy="2.42" r=".4" />
        <circle cx="47.8" cy="2.42" r=".4" />

        <g stroke-width=".1" paint-order="fill stroke">
          <circle
            id="reset-button"
            cx="49.7"
            cy="4.3"
            r="1.4"
            fill="#000"
            stroke="#3f3f3f"
            tabindex="0"
            @mousedown=${()=>this.down()}
            @touchstart=${()=>this.down()}
            @mouseup=${()=>this.up()}
            @mouseleave=${()=>this.leave()}
            @touchend=${()=>this.leave()}
            @keydown=${s=>Te.includes(s.key)&&this.down()}
            @keyup=${s=>Te.includes(s.key)&&this.up()}
          />
        </g>

        <!-- Logo -->
        <g fill="#fff">
          <path d="M55.5 25.1h-3.54v2.42h3.54V25.1zm-.174.174v2.07h-3.19v-2.07h3.19z" />
          <path
            d="M56 23.6a.514.514 0 00-.514-.514h-14a.514.514 0 00-.514.514v1.03c0 .283.23.514.514.514h14A.515.515 0 0056 24.63V23.6z"
          />
        </g>
        <text
          x="41.14"
          y="24.9"
          fill="#2c8240"
          style="font-family: sans-serif; font-weight: bold; font-size: 2.15"
        >
          FRANZININHO
        </text>
        <text
          x="52.2"
          y="26.95"
          fill="#fff"
          style="font-family: sans-serif; font-weight: bold; font-size: 1.78"
        >
          DIY
        </text>
      </svg>
    `}down(){this.resetPressed||(this.resetPressed=!0,this.resetButton.style.stroke="#333",this.dispatchEvent(new CustomEvent("button-press",{detail:"reset"})))}up(){!this.resetPressed||(this.resetPressed=!1,this.resetButton.style.stroke="",this.dispatchEvent(new CustomEvent("button-release",{detail:"reset"})))}leave(){this.resetButton.blur(),this.up()}};k1([y()],Ct.prototype,"led1",void 0);k1([y()],Ct.prototype,"ledPower",void 0);k1([y()],Ct.prototype,"resetPressed",void 0);k1([v1("#reset-button")],Ct.prototype,"resetButton",void 0);Ct=k1([U("wokwi-franzininho")],Ct);var Rt=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let J0=class extends L{constructor(){super(...arguments);this.ledRed=0,this.ledGreen=0,this.ledBlue=0,this.ledBuiltIn=!1,this.ledPower=!1,this.pinInfo=[{name:"D12",x:20.1,y:1,signals:[te("MISO"),{type:"pwm"}],description:"GPIO04"},{name:"D11",x:29.8,y:1,signals:[te("MOSI"),{type:"pwm"}],description:"GPIO07"},{name:"D10",x:39.3,y:1,signals:[{type:"pwm"}],description:"GPIO05"},{name:"D9",x:48.9,y:1,signals:[{type:"pwm"}],description:"GPIO21"},{name:"D8",x:58.5,y:1,signals:[{type:"pwm"}],description:"GPIO20"},{name:"D7",x:68.1,y:1,signals:[{type:"pwm"}],description:"GPIO19"},{name:"D6",x:77.7,y:1,signals:[{type:"pwm"}],description:"GPIO18"},{name:"D5",x:87.3,y:1,signals:[{type:"pwm"}],description:"GPIO17"},{name:"D4",x:96.9,y:1,signals:[{type:"pwm"}],description:"GPIO16"},{name:"D3",x:106.5,y:1,signals:[{type:"pwm"}],description:"GPIO15"},{name:"D2",x:116.1,y:1,signals:[{type:"pwm"}],description:"GPIO25"},{name:"GND.1",x:125.2,y:1,signals:[{type:"power",signal:"GND"}]},{name:"RESET",x:135.3,y:1,signals:[]},{name:"RX",x:153.9,y:1,signals:[ze("RX")],description:"GPIO1"},{name:"TX",x:144.5,y:1,signals:[ze("TX")],description:"GPIO0"},{name:"D13",x:20.1,y:67.5,signals:[te("SCK")],description:"GPIO6"},{name:"3.3V",x:29.7,y:67.5,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"AREF",x:39.3,y:67.5,signals:[]},{name:"A0",x:48.8,y:67.5,signals:[H(0)],description:"GPIO26"},{name:"A1",x:58.5,y:67.5,signals:[H(1)],description:"GPIO27"},{name:"A2",x:68,y:67.5,signals:[H(2)],description:"GPIO28"},{name:"A3",x:77.6,y:67.5,signals:[H(3)],description:"GPIO29"},{name:"A4",x:87.3,y:67.5,signals:[H(4)],description:"GPIO12"},{name:"A5",x:96.9,y:67.5,signals:[H(5)],description:"GPIO13"},{name:"A6",x:106.5,y:67.5,signals:[H(6)]},{name:"A7",x:116.1,y:67.5,signals:[H(7)]},{name:"5V",x:125.5,y:67.5,signals:[{type:"power",signal:"VCC",voltage:5}]},{name:"RESET.2",x:134.9,y:67.5,signals:[]},{name:"GND.2",x:145.3,y:67.5,signals:[{type:"power",signal:"GND"}]},{name:"VIN",x:154.1,y:67.5,signals:[{type:"power",signal:"VCC"}]}]}render(){const{ledPower:e,ledBuiltIn:i,ledRed:s,ledGreen:n,ledBlue:r}=this,l=Math.max(s,n,r),o=l?.3+l*.7:0;return G`
      <svg
        width="44.573mm"
        height="17.956mm"
        clip-rule="evenodd"
        fill-rule="evenodd"
        version="1.1"
        viewBox="0 0 168 67.9"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pad-pattern" height="10" width="9.58" patternUnits="userSpaceOnUse">
            <path
              d="m5.88 0.00992v5.57c0 1.63-1.32 2.95-2.94 2.95h-0.0025c-1.63 0-2.94-1.32-2.94-2.95v-5.57h0.805c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14zm-2.95 7.65c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14z"
              fill="#ffdc8e"
              stroke-width="1.24"
            />
          </pattern>
          <pattern id="pin-pattern" height="10" width="1.3" patternUnits="userSpaceOnUse">
            <path
              d="m0.5 0c-0.205 0-0.37 0.165-0.37 0.37v1.08h0.739v-1.08c0-0.205-0.165-0.37-0.37-0.37z"
              fill="#eaecec"
            />
          </pattern>
        </defs>

        <!-- Board -->
        <path
          d="m156 0h12.2v67.9h-12.2c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.3c0-1.18-0.959-2.14-2.14-2.14-1.18 0-2.14 0.96-2.14 2.14h-5.29c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-5.29c0-1.18-0.96-2.14-2.14-2.14s-2.14 0.96-2.14 2.14h-12.2v-67.9h12.2v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.959 2.14 2.14 2.14 1.18 0 2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.959 2.14 2.14 2.14 1.18 0 2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.959 2.14 2.14 2.14 1.18 0 2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.959 2.14 2.14 2.14 1.18 0 2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.3v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14v-0.00992h5.29v0.00992c0 1.18 0.959 2.14 2.14 2.14 1.18 0 2.14-0.96 2.14-2.14v-0.00992h5.29v0.00992c0 1.18 0.96 2.14 2.14 2.14s2.14-0.96 2.14-2.14zm7.38 58.8c1.97 0 3.56 1.6 3.56 3.56 0 1.97-1.6 3.56-3.56 3.56-1.97 0-3.56-1.6-3.56-3.56 0-1.97 1.6-3.56 3.56-3.56zm-153 0c1.97 0 3.56 1.6 3.56 3.56 0 1.97-1.6 3.56-3.56 3.56-1.97 0-3.56-1.6-3.56-3.56 0-1.97 1.6-3.56 3.56-3.56zm67 1.42c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-9.58 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-47.9 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm76.6 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-67.1 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.959-2.14 2.14-2.14zm19.2 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-9.58 0c1.18 0 2.14 0.96 2.14 2.14s-0.959 2.14-2.14 2.14c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14zm47.9 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.959-2.14 2.14-2.14zm-28.8 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.959-2.14 2.14-2.14zm86.3 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-38.3 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm19.2 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm9.58 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-19.2 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.959-2.14 2.14-2.14zm38.3 0c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14-2.14-0.96-2.14-2.14 0.96-2.14 2.14-2.14zm-144-58.3c1.97 0 3.56 1.6 3.56 3.56 0 1.97-1.6 3.56-3.56 3.56-1.97 0-3.56-1.6-3.56-3.56 0-1.97 1.6-3.56 3.56-3.56zm153 0c1.97 0 3.56 1.6 3.56 3.56 0 1.97-1.6 3.56-3.56 3.56-1.97 0-3.56-1.6-3.56-3.56 0-1.97 1.6-3.56 3.56-3.56zm-28.1 5.71c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm-19.2 0c-1.18 0-2.14-0.96-2.14-2.14s0.959-2.14 2.14-2.14c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm-28.8 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm67.1 0c-1.18 0-2.14-0.96-2.14-2.14s0.959-2.14 2.14-2.14c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14zm9.57 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.959 2.14-2.14 2.14zm-57.5 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.959-2.14 2.14-2.14c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14zm-19.2 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.959-2.14 2.14-2.14c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm76.7 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14zm-86.3 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.959 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.959-2.14 2.14-2.14c1.18 0 2.14 0.96 2.14 2.14s-0.96 2.14-2.14 2.14zm-9.58 0c-1.18 0-2.14-0.96-2.14-2.14s0.96-2.14 2.14-2.14 2.14 0.96 2.14 2.14-0.96 2.14-2.14 2.14z"
          fill="#1a466b"
        />
        <path
          d="m148 67.9v-5.57c0-1.9-1.54-3.44-3.44-3.44h-2e-3c-1.9 0-3.44 1.54-3.44 3.44v5.57h-1.47v-9.55h9.88v9.55zm-0.756-6.78 7e-3 0.0161c-0.459-1.03-1.49-1.75-2.69-1.75h-2e-3c-0.661 0-1.27 0.218-1.76 0.587 0.492-0.368 1.1-0.587 1.76-0.587h2e-3c1.19 0 2.22 0.712 2.68 1.73zm-4.55-1.07c0.0186-0.0161 0.0372-0.031 0.0558-0.0459-0.0186 0.0149-0.0372 0.0298-0.0558 0.0459zm-0.0942 0.0794c0.0124-0.0112 0.026-0.0236 0.0397-0.0347l0.0136-0.0112c-0.0186 0.0149-0.036 0.031-0.0533 0.0459zm-0.343 0.36c0.0744-0.0918 0.154-0.18 0.239-0.263-0.0856 0.0831-0.165 0.171-0.239 0.263zm-0.0967 0.129c0.0236-0.0347 0.0496-0.0682 0.0756-0.1-0.026 0.0322-0.0521 0.0657-0.0756 0.1zm-0.0409 0.057c9e-3 -0.0124 0.0174-0.0248 0.026-0.036-9e-3 0.0112-0.0174 0.0236-0.026 0.036zm-0.18 0.301v-0.0012 0.0012l-7e-3 0.0124zm5.56 6.93v-5.57c0-0.423-0.0893-0.825-0.25-1.19 0.161 0.365 0.25 0.766 0.25 1.19zm-5.82-6.24c5e-3 -0.0223 0.01-0.0446 0.0161-0.067-6e-3 0.0223-0.0112 0.0446-0.0161 0.067zm-4e-3 0.0136v0.0037z"
          fill="#fff"
        />

        <!-- Flash memory chip -->
        <path
          d="m59.9 14.4c0-0.394-0.32-0.714-0.714-0.714h-0.738c-0.396 0-0.715 0.32-0.715 0.714v0.0012h2.17z"
          fill="#eaecec"
        />
        <path
          d="m59.9 14.4c0-0.394-0.32-0.714-0.714-0.714h-0.738c-0.396 0-0.715 0.32-0.715 0.714v0.0012h2.17z"
          fill="#1d1d1b"
        />
        <path
          d="m63 14.4c0-0.394-0.32-0.714-0.715-0.714h-0.738c-0.394 0-0.715 0.32-0.715 0.714v0.0012h2.17z"
          fill="#eaecec"
        />
        <path
          d="m63 14.4c0-0.394-0.32-0.714-0.715-0.714h-0.738c-0.394 0-0.715 0.32-0.715 0.714v0.0012h2.17z"
          fill="#1d1d1b"
        />
        <path
          d="m66 14.4c0-0.394-0.32-0.714-0.714-0.714h-0.739c-0.394 0-0.714 0.32-0.714 0.714v0.0012h2.17z"
          fill="#eaecec"
        />
        <path
          d="m66 14.4c0-0.394-0.32-0.714-0.714-0.714h-0.739c-0.394 0-0.714 0.32-0.714 0.714v0.0012h2.17z"
          fill="#1d1d1b"
        />
        <path
          d="m68.9 14.4c0-0.394-0.321-0.714-0.715-0.714h-0.738c-0.396 0-0.715 0.32-0.715 0.714v0.0012h2.17z"
          fill="#eaecec"
        />
        <path
          d="m68.9 14.4v-0.0012c0-0.394-0.321-0.714-0.715-0.714h-0.738c-0.396 0-0.715 0.32-0.715 0.714v0.0012z"
          fill="#1d1d1b"
        />
        <path
          d="m66.8 32.1c0 0.394 0.32 0.714 0.715 0.714h0.738c0.394 0 0.715-0.32 0.715-0.714z"
          fill="#eaecec"
        />
        <path
          d="m68.9 32.2c-0.0384 0.206-0.165 0.379-0.339 0.482 0.113-0.169 0.226-0.331 0.339-0.482z"
          fill="#1d1d1b"
        />
        <path
          d="m63.8 32.1c0 0.394 0.32 0.714 0.714 0.714h0.739c0.394 0 0.714-0.32 0.714-0.714z"
          fill="#eaecec"
        />
        <path
          d="m66 32.2c-0.0384 0.206-0.165 0.379-0.339 0.482 0.113-0.169 0.226-0.331 0.339-0.482z"
          fill="#1d1d1b"
        />
        <path
          d="m60.7 32.1c0 0.394 0.321 0.714 0.715 0.714h0.738c0.396 0 0.715-0.32 0.715-0.714z"
          fill="#eaecec"
        />
        <path
          d="m62.9 32.2c-0.0384 0.206-0.165 0.379-0.339 0.482 0.113-0.169 0.227-0.331 0.339-0.482z"
          fill="#1d1d1b"
        />
        <path
          d="m57.8 32.1c0 0.394 0.32 0.714 0.715 0.714h0.738c0.394 0 0.714-0.32 0.714-0.714z"
          fill="#eaecec"
        />
        <path
          d="m59.9 32.2c-0.0384 0.206-0.165 0.379-0.339 0.482 0.113-0.169 0.226-0.331 0.339-0.482z"
          fill="#1d1d1b"
        />
        <rect x="56.7" y="14.1" width="13.3" height="18.4" fill="#3b3838" />

        <!-- Mounting holes -->
        <g fill="none" stroke="#ffdc8e" stroke-width=".496px">
          <circle cx="10.4" cy="62.4" r="3.56" />
          <circle cx="163.5" cy="62.4" r="3.56" />
          <circle cx="10.2" cy="5.52" r="3.56" />
          <circle cx="163" cy="5.52" r="3.56" />
        </g>

        <!-- Pads -->
        <rect transform="translate(17, 0)" width="142" height="9.5" fill="url(#pad-pattern)" />
        <rect
          transform="translate(17, 68) scale(1,-1)"
          width="142"
          height="9.5"
          fill="url(#pad-pattern)"
        />

        <!-- RP2040 -->
        <rect
          transform="translate(79,37.8) scale(1,-1)"
          width="19.5"
          height="2"
          fill="url(#pin-pattern)"
        />
        <rect transform="translate(79,12.5)" width="19.5" height="2" fill="url(#pin-pattern)" />
        <rect
          transform="translate(102.3,15.5) rotate(90)"
          width="19.5"
          height="2"
          fill="url(#pin-pattern)"
        />
        <rect
          transform="translate(75.5,34.5) rotate(270)"
          width="19.5"
          height="2"
          fill="url(#pin-pattern)"
        />

        <!-- Accelerometer chip -->
        <rect transform="translate(90,42)" width="3.8" height="2" fill="url(#pin-pattern)" />
        <rect
          transform="translate(90,57.5) scale(1,-1)"
          width="3.8"
          height="2"
          fill="url(#pin-pattern)"
        />
        <rect
          transform="translate(100,47.5) rotate(90)"
          width="4.8"
          height="2"
          fill="url(#pin-pattern)"
        />
        <rect
          transform="translate(84,52.5) rotate(270)"
          width="4.8"
          height="2"
          fill="url(#pin-pattern)"
        />

        <!-- Crypto chip -->
        <rect transform="translate(104,40)" width="4.8" height="2" fill="url(#pin-pattern)" />
        <rect
          transform="translate(104,57) scale(1,-1)"
          width="4.8"
          height="2"
          fill="url(#pin-pattern)"
        />

        <rect x="85.3" y="43.7" width="13.4" height="12.4" fill="#3b3838" stroke-width="1.24" />

        <rect x="76.9" y="14.1" width="23.8" height="22.2" fill="#3b3838" />

        <!-- Microphone -->
        <g stroke-width="1.24">
          <path d="m52.7 25.8v-11.7h-16.1v11.7z" fill="#3b3838" />
          <circle cx="46.7" cy="19.9" r="2.77" fill="#3b3838" />
          <path
            d="m46.7 17.1c1.53 0 2.78 1.24 2.78 2.78 0 1.53-1.24 2.78-2.78 2.78-1.53 0-2.78-1.24-2.78-2.78 0-1.53 1.24-2.78 2.78-2.78zm0 3.72c-0.523 0-0.949-0.425-0.949-0.949s0.425-0.949 0.949-0.949 0.949 0.425 0.949 0.949-0.425 0.949-0.949 0.949z"
            fill="#ffdc8e"
          />
        </g>

        <path d="m111 55.5v-14h-9.03v14z" fill="#3b3838" />

        <!-- WiFi module -->
        <rect x="112" y="10.1" width="56" height="45.4" fill="#418e54" />
        <path
          d="m160 53.6h-3.05v-41.9h11.3v41.9h-2.36c0-0.764-0.308-1.38-0.687-1.38-0.378 0-0.686 0.619-0.686 1.38h-3.17c0-0.764-0.308-1.38-0.687-1.38s-0.687 0.619-0.687 1.38z"
          fill="#cecccb"
        />
        <rect x="115" y="11.7" width="41.9" height="41.9" fill="#e4e4e4" />
        <path d="m157 11.7h-41.9v41.9h41.9zm-0.868 0.868v40.2h-40.2v-40.2z" fill="#ffdc8e" />
        <path
          d="m165 11.2h-3.86v35.3c0 0.0868 0.0347 0.171 0.0967 0.233s0.146 0.098 0.234 0.098h3.19c0.0868 0 0.171-0.036 0.233-0.098s0.098-0.146 0.098-0.233v-35.3z"
          fill="#448f53"
        />

        <!-- USB connector -->
        <path
          d="m3.2 47.5-2.18 1.07c-0.138 0.0657-0.295 0.0756-0.439 0.026-0.145-0.0496-0.263-0.154-0.33-0.291-0.067-0.136-0.0769-0.295-0.0273-0.439s0.154-0.263 0.291-0.33l1.5-0.73h-0.203c-0.479 0-0.939-0.19-1.28-0.529-0.34-0.339-0.529-0.799-0.529-1.28v-21.8c0-0.48 0.19-0.939 0.529-1.28 0.339-0.339 0.799-0.529 1.28-0.529h0.203l-1.5-0.73c-0.138-0.067-0.242-0.185-0.291-0.33-0.0496-0.144-0.0397-0.301 0.0273-0.438v-0.0012c0.067-0.136 0.185-0.242 0.33-0.291 0.144-0.0496 0.301-0.0397 0.439 0.0273l2.18 1.06v-0.591h20.2v27.7h-20.2z"
          fill="#cecccb"
        />
        <path
          d="m23.4 41.7h-1.77c-0.0595 0-0.113 0.031-0.144 0.0806-0.316 0.516-2.04 3.34-2.04 3.34h-1.63v-1.71h-1.17v-2.58h1.17v-1.63h3.1c0.133 0 0.259-0.0521 0.352-0.145 0.093-0.0942 0.145-0.221 0.145-0.352v-1.55h-1.29v-6.34h1.29v-1.55c0-0.131-0.0521-0.258-0.145-0.351-0.093-0.0942-0.219-0.146-0.352-0.146h-3.1v-1.63h-1.17v-2.58h1.17v-1.71h1.63s1.73 2.83 2.04 3.34c0.031 0.0508 0.0843 0.0806 0.144 0.0806h1.77z"
          fill="#989898"
        />

        <!-- Reset button -->
        <g stroke-width="1.24">
          <rect x="42.9" y="32.7" width="11.1" height="9.96" fill="#cecccb" />
          <circle cx="48.5" cy="37.7" r="3.48" fill="#ffdc8e" />
          <g fill="#cecccb">
            <path d="m46 44.9h-1.34v1.64c0 0.734 0.595 1.33 1.33 1.33h0.0099z" />
            <path d="m46 30.7h-1.34v-1.64c0-0.734 0.595-1.33 1.33-1.33h0.0099z" />
            <rect x="46.2" y="45.2" width="5.23" height="2.59" />
            <rect x="46.2" y="27.7" width="5.23" height="2.59" />
            <path
              d="m49.7 30.6c0.062 0 0.122 0.0248 0.166 0.0682 0.0434 0.0446 0.0682 0.104 0.0682 0.167 0 0.134 0.0533 0.263 0.149 0.358 0.0955 0.0942 0.224 0.148 0.358 0.148h0.0236c0.141 0 0.277-0.0558 0.376-0.155s0.155-0.234 0.155-0.374v-0.564h2.16v3.09h-1.69v0.744h-2.16v-0.392h-1.87v0.392h-2.16v-0.744h-1.69v-3.09h2.16v0.564c0 0.14 0.0558 0.275 0.155 0.374s0.234 0.155 0.376 0.155h0.0236c0.135 0 0.263-0.0533 0.358-0.148 0.0955-0.0955 0.149-0.224 0.149-0.358 0-0.0632 0.0248-0.123 0.0682-0.167 0.0446-0.0434 0.104-0.0682 0.167-0.0682z"
            />
            <path
              d="m47.1 44.9c-0.0632 0-0.123-0.0248-0.167-0.0694-0.0434-0.0434-0.0682-0.103-0.0682-0.166 0-0.134-0.0533-0.263-0.149-0.358s-0.223-0.149-0.358-0.149h-0.0236c-0.141 0-0.277 0.0558-0.376 0.156-0.0992 0.0992-0.155 0.234-0.155 0.374v0.564h-2.16v-3.09h1.69v-0.745h2.16v0.393h1.87v-0.393h2.16v0.745h1.69v3.09h-2.16v-0.564c0-0.14-0.0558-0.275-0.155-0.374-0.0992-0.1-0.234-0.156-0.376-0.156h-0.0236c-0.134 0-0.263 0.0533-0.358 0.149s-0.149 0.224-0.149 0.358c0 0.0632-0.0248 0.123-0.0682 0.166-0.0446 0.0446-0.104 0.0694-0.166 0.0694z"
            />
          </g>
        </g>

        <!-- Arduino Logo -->
        <path
          d="m135 33.7c-0.911 0.472-2.46 1.57-3.14 2.23-0.718 0.699-1.3 1.49-1.5 2.49-0.219 1.13-0.187 2.26 0.207 3.35 0.642 1.77 2.29 3.03 4.09 3.09 1.95 0.0707 3.7-0.982 4.44-2.8 0.851-2.08 0.553-4.05-0.885-5.8-0.885-1.08-2.05-1.84-3.2-2.57m-0.0223-3.06c1.35-0.861 2.68-1.72 3.58-3.06 1.05-1.54 1.24-3.22 0.656-4.98-0.624-1.86-2.27-3.09-4.2-3.15-1.9-0.0583-3.48 1.03-4.28 2.68-0.391 0.808-0.495 1.67-0.505 2.55-0.0149 1.26 0.384 2.36 1.23 3.29 0.998 1.11 2.26 1.87 3.52 2.67m2.43 1.5c0.605 0.49 1.2 0.934 1.76 1.43 1.45 1.29 2.44 2.85 2.78 4.77 0.541 3.09-0.298 5.73-2.83 7.66-3.65 2.79-8.84 1.41-10.8-2.83-1.25-2.77-1.04-6.6 1.76-9.15 0.627-0.572 1.33-1.06 2-1.59 0.114-0.0905 0.239-0.167 0.403-0.28-0.186-0.13-0.336-0.238-0.49-0.341-1.01-0.686-1.96-1.44-2.73-2.41-1.21-1.51-1.72-3.24-1.66-5.17 0.0533-1.62 0.494-3.12 1.48-4.41 1.65-2.17 3.84-3.24 6.57-2.9 2.85 0.345 4.86 1.96 5.85 4.63 1.19 3.17 0.508 6.06-1.76 8.56-0.593 0.653-1.34 1.17-2.01 1.75-0.112 0.0955-0.238 0.174-0.379 0.275"
          fill-rule="nonzero"
        />
        <path d="m137 22.3v4.51h-1.44v-4.51z" />
        <path
          d="m137 37.4v1.34h1.34v1.62h-1.34v1.36h-1.62v-1.34h-1.35v-1.64h1.34v-1.35z"
          fill-rule="nonzero"
        />

        <!-- LEDs -->
        <g stroke-width="1.24">
          <rect x="8.47" y="12.6" width="11.9" height="4.06" fill="#a19e9e" />
          <rect x="8.47" y="50.9" width="11.9" height="4.06" fill="#a19e9e" />

          <!-- LED BUILTIN -->
          <rect x="11.9" y="12.6" width="4.94" height="4.06" fill="#f1d99f" />

          <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
            <feGaussianBlur stdDeviation="2" />
          </filter>

          ${i&&V`<circle cx="14.5" cy="14.5" r="3" fill="red" filter="url(#ledFilter)" />`}

          <!-- LED POWER -->
          <rect x="11.9" y="50.9" width="4.94" height="4.06" fill="#f1d99f" />
          ${e&&V`<circle cx="14.5" cy="53" r="3" fill="#80ff80" filter="url(#ledFilter)" />`}

          <!-- LED RGB -->
          <g fill="#ffdc8e">
            <rect x="33.4" y="23.6" width="1.25" height="1.25" />
            <rect x="30.2" y="23.6" width="1.25" height="1.25" />
            <rect x="33.4" y="26.8" width="1.25" height="1.25" />
            <rect x="30.2" y="26.8" width="1.25" height="1.25" />
          </g>
          <rect x="30.8" y="24.1" width="3.4" height="3.4" fill="#cecccb" />

          <circle
            cx="32.4"
            cy="25.4"
            r="3"
            fill="rgb(${s*255}, ${n*255}, ${r*255})"
            filter="url(#ledFilter)"
            opacity="${o}"
          />

          <path
            d="m122 0.00992v5.57c0 1.9 1.54 3.44 3.44 3.44h1e-3c1.9 0 3.44-1.54 3.44-3.44v-5.57h1.47v9.55h-9.88v-9.55zm0.756 6.78-7e-3 -0.0161c0.459 1.03 1.49 1.75 2.69 1.75h1e-3c0.661 0 1.27-0.218 1.76-0.587-0.491 0.368-1.1 0.587-1.76 0.587h-1e-3c-1.2 0-2.22-0.712-2.69-1.73zm4.55 1.07c-0.0186 0.0161-0.0384 0.031-0.057 0.0459 0.0186-0.0149 0.0384-0.0298 0.057-0.0459zm0.0942-0.0794c-0.0136 0.0112-0.0273 0.0236-0.0397 0.0347l-0.0136 0.0112c0.0174-0.0149 0.036-0.031 0.0533-0.0459zm0.342-0.36c-0.0744 0.0918-0.154 0.18-0.239 0.263 0.0856-0.0831 0.165-0.171 0.239-0.263zm0.098-0.129c-0.0248 0.0347-0.0496 0.0682-0.0756 0.1 0.026-0.0322 0.0508-0.0657 0.0756-0.1zm0.0397-0.057c-9e-3 0.0124-0.0161 0.0248-0.0248 0.036 9e-3 -0.0112 0.0161-0.0236 0.0248-0.036zm0.181-0.301-1e-3 0.00124 1e-3 -0.00124 6e-3 -0.0124zm-5.56-6.93v5.57c0 0.424 0.0893 0.826 0.25 1.19-0.161-0.365-0.25-0.766-0.25-1.19zm5.82 6.24c-5e-3 0.0223-0.01 0.0446-0.0161 0.067 6e-3 -0.0223 0.0112-0.0446 0.0161-0.067zm4e-3 -0.0136 1e-3 -0.00372z"
            fill="#fff"
          />
        </g>
      </svg>
    `}};Rt([y()],J0.prototype,"ledRed",void 0);Rt([y()],J0.prototype,"ledGreen",void 0);Rt([y()],J0.prototype,"ledBlue",void 0);Rt([y()],J0.prototype,"ledBuiltIn",void 0);Rt([y()],J0.prototype,"ledPower",void 0);J0=Rt([U("wokwi-nano-rp2040-connect")],J0);var tn=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let Z1=class extends L{constructor(){super(...arguments);this.ledPower=!1,this.ledSignal=!1,this.pinInfo=[{name:"AOUT",y:11,x:0,number:1,signals:[]},{name:"GND",y:20.5,x:0,number:2,signals:[$e()]},{name:"VCC",y:30.5,x:0,number:3,signals:[De()]},{name:"DOUT",y:40.5,x:0,number:4,signals:[]}]}render(){const{ledPower:e,ledSignal:i}=this;return G`
      <svg
        width="35.211mm"
        height="13.346mm"
        clip-rule="evenodd"
        fill-rule="evenodd"
        version="1.1"
        viewBox="0 0 133 50.4"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="3.6" width="14" patternUnits="userSpaceOnUse">
            <path
              d="m5.09 1.3c0-0.618-0.502-1.12-1.12-1.12h-3.97v2.6h3.97c0.618 0 1.12-0.502 1.12-1.12z"
              fill="#eaecec"
            />
            <path
              d="m5.09 1.3c0-0.297-0.118-0.582-0.328-0.792-0.211-0.21-0.496-0.328-0.793-0.328h-1.14v2.6h1.14c0.297 0 0.582-0.118 0.793-0.328 0.21-0.21 0.328-0.495 0.328-0.793z"
              fill="#adafb0"
            />
          </pattern>
          <g id="header-pin">
            <path d="m3.4 1.8v-6.55h-6.55v6.55z" fill="#433b38" />
            <path
              d="m0 0c0.382 0 0.748-0.152 1.02-0.422 0.27-0.27 0.421-0.637 0.421-1.02s-0.151-0.748-0.421-1.02c-0.271-0.27-0.637-0.422-1.02-0.422h-26.1c-0.233 0-0.423 0.19-0.423 0.424v2.04c0 0.233 0.19 0.423 0.423 0.423h26.1z"
              fill="#9f9f9f"
            />
          </g>
        </defs>

        <!-- Board -->
        <path
          d="m133 8.69v-8.69h-113v50.4h113v-8.69h-10.9c-0.444 0-0.804-0.36-0.804-0.804v-31.5c0-0.444 0.36-0.804 0.804-0.804zm-84.3 8.1c4.65 0 8.43 3.78 8.43 8.43 0 4.65-3.78 8.43-8.43 8.43s-8.43-3.78-8.43-8.43c0-4.65 3.78-8.43 8.43-8.43z"
          fill="#931917"
        />

        <!-- Chip  -->
        <rect transform="translate(87,26)" width="5" height="14.5" fill="url(#pin-pattern)" />
        <rect
          transform="translate(74,40) rotate(180)"
          width="5"
          height="14.5"
          fill="url(#pin-pattern)"
        />

        <rect x="73.5" y="25.2" width="13.9" height="15.6" fill="#3b3838" />
        <path
          d="m88.7 40.9v3.24h-6.63c0-1.02-0.721-1.85-1.61-1.85s-1.61 0.827-1.61 1.85h-6.62v-3.24"
          fill="none"
          stroke="#fff"
          stroke-width=".4px"
        />
        <path d="m72.2 25.2v-1.74h16.5v1.74" fill="none" stroke="#fff" stroke-width=".4px" />

        <!-- PCB pins -->
        <path d="m31 44.6v-38h-8.39v38z" fill="none" stroke="#fff" stroke-width=".4px" />
        <use xlink:href="#header-pin" x="26.6" y="12.4" />
        <use xlink:href="#header-pin" x="26.6" y="22.1" />
        <use xlink:href="#header-pin" x="26.6" y="31.9" />
        <use xlink:href="#header-pin" x="26.6" y="41.6" />

        <path
          d="m48.8 13.9c6.24 0 11.3 5.07 11.3 11.3 0 6.24-5.07 11.3-11.3 11.3s-11.3-5.07-11.3-11.3c0-6.24 5.07-11.3 11.3-11.3zm0 2.88c4.65 0 8.43 3.78 8.43 8.43 0 4.65-3.78 8.43-8.43 8.43s-8.43-3.78-8.43-8.43c0-4.65 3.78-8.43 8.43-8.43z"
          fill="#d6d6d2"
        />
        <rect x="65.3" y="2.76" width="28.2" height="14.2" fill="#4875ce" />
        <path
          d="m94.9 2.1c0-0.221-0.179-0.4-0.4-0.4h-30.1c-0.221 0-0.4 0.179-0.4 0.4v15.5c0 0.221 0.179 0.4 0.4 0.4h30.1c0.221 0 0.4-0.179 0.4-0.4zm-0.4 0h-30.1v15.5h30.1z"
          fill="#fff"
        />
        <circle cx="69.9" cy="6.54" r="2.31" fill="#f1d99f" />
        <path
          d="m69.3 4.34c0.196-0.066 0.405-0.096 0.622-0.096 0.216 0 0.426 0.03 0.622 0.096v4.42c-0.196 0.066-0.406 0.096-0.622 0.096-0.217 0-0.426-0.03-0.622-0.096z"
          fill="#a4987a"
        />

        <!-- LEDs -->
        <rect
          x="50.5"
          y="1.4"
          width="11.5"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="51.5" y="2.12" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="54.3" y="2.12" width="3.98" height="3.28" fill="#f1d99f" />

        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="2" />
        </filter>
        ${e&&V`<circle cx="56" cy="4" r="7" fill="#80ff80" filter="url(#ledFilter)" />`}

        <rect
          x="50.5"
          y="44.4"
          width="11.5"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="51.5" y="45.1" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="54.3" y="45.1" width="3.98" height="3.28" fill="#f1d99f" />

        ${i&&V`<circle cx="56" cy="47" r="7" fill="#80ff80" filter="url(#ledFilter)" />`}

        <!-- Resistors -->
        <rect
          x="32.3"
          y="1.4"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="33.2" y="2.12" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="36" y="2.52" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="1.92"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="2.64" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="3.05" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="43.9"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="44.6" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="45" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="9"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="9.72" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="10.1" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="36.8"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="37.6" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="38" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="32.3"
          y="44.4"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="33.2" y="45.1" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="36" y="45.5" width="3.97" height="2.47" fill="#171514" />

        <text fill="#ffffff" font-family="sans-serif" font-size="3.72px">
          <tspan x="33.049" y="12.085">AO</tspan>
          <tspan x="33.049" y="41.951">DO</tspan>
          <tspan x="33.049" y="22.067">G</tspan>
          <tspan x="32.964001" y="32.418999" font-size="5.25px">+</tspan>
          <tspan x="52.675778" y="9.31008">PWR</tspan>
          <tspan x="52.675778" y="13.100851">LED</tspan>
          <tspan x="52.675778" y="40">DO</tspan>
          <tspan x="52.675778" y="43.4">LED</tspan>
        </text>

        <path
          d="m110 24.7c0-1.29-1.05-2.34-2.34-2.34h-1e-3c-1.29 0-2.34 1.05-2.34 2.34v0.989c0 1.3 1.05 2.34 2.34 2.34h1e-3c1.29 0 2.34-1.05 2.34-2.34v-0.989z"
          fill="#f8f3e9"
        />
        <circle cx="108" cy="25.2" r="1.14" fill="#3b3838" />
        <path
          d="m115 6.29c1.29 0 2.34-1.05 2.34-2.34v-1e-3c0-1.29-1.05-2.34-2.34-2.34h-0.99c-1.29 0-2.34 1.05-2.34 2.34v1e-3c0 1.29 1.05 2.34 2.34 2.34h0.99z"
          fill="#f8f3e9"
        />
        <path
          d="m116 3.95c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.512-1.14-1.14c0-0.632 0.512-1.14 1.14-1.14s1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m129 47.7c0.621 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66 0-0.621-0.247-1.22-0.686-1.66-0.44-0.44-1.04-0.687-1.66-0.687h-3.03c-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66 0 0.622 0.247 1.22 0.687 1.66 0.439 0.439 1.04 0.686 1.66 0.686z"
          fill="#f8f3e9"
        />
        <path
          d="m128 45.4c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.513-1.14-1.14 0.512-1.14 1.14-1.14 1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m129 6.29c0.621 0 1.22-0.247 1.66-0.687 0.439-0.439 0.686-1.04 0.686-1.66v-1e-3c0-0.621-0.247-1.22-0.686-1.66-0.44-0.439-1.04-0.686-1.66-0.686h-3.03c-0.621 0-1.22 0.247-1.66 0.686-0.44 0.44-0.687 1.04-0.687 1.66v1e-3c0 0.621 0.247 1.22 0.687 1.66 0.439 0.44 1.04 0.687 1.66 0.687z"
          fill="#f8f3e9"
        />
        <path
          d="m128 3.95c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.512-1.14-1.14c0-0.632 0.512-1.14 1.14-1.14s1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m117 41.4c0.621 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66s-0.247-1.22-0.686-1.66c-0.44-0.44-1.04-0.687-1.66-0.687h-3.03c-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66s0.247 1.22 0.687 1.66c0.439 0.439 1.04 0.686 1.66 0.686z"
          fill="#dddcdb"
        />
        <path
          d="m117 9.6c0.621 0 1.22 0.247 1.66 0.686 0.439 0.44 0.686 1.04 0.686 1.66 0 0.621-0.247 1.22-0.686 1.66-0.44 0.44-1.04 0.687-1.66 0.687h-3.03c-0.621 0-1.22-0.247-1.66-0.687-0.44-0.439-0.687-1.04-0.687-1.66 0-0.622 0.247-1.22 0.687-1.66 0.439-0.439 1.04-0.686 1.66-0.686z"
          fill="#f8f3e9"
        />
        <path
          d="m114 11.9c0-0.631-0.512-1.14-1.14-1.14-0.631 0-1.14 0.513-1.14 1.14s0.512 1.14 1.14 1.14c0.632 0 1.14-0.512 1.14-1.14z"
          fill="#3b3838"
        />
        <circle cx="122" cy="23.7" r="11.3" fill="#d6d6d2" />
        <circle cx="122" cy="23.7" r="10.2" fill="#3b3838" />
        <path
          d="m114 39.1c0 0.631-0.512 1.14-1.14 1.14-0.631 0-1.14-0.513-1.14-1.14s0.512-1.14 1.14-1.14c0.632 0 1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
      </svg>
    `}};tn([y()],Z1.prototype,"ledPower",void 0);tn([y()],Z1.prototype,"ledSignal",void 0);Z1=tn([U("wokwi-small-sound-sensor")],Z1);var sn=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let J1=class extends L{constructor(){super(...arguments);this.led1=!1,this.led2=!1,this.pinInfo=[{name:"AOUT",y:11,x:0,number:1,signals:[]},{name:"GND",y:20.5,x:0,number:2,signals:[$e()]},{name:"VCC",y:30.5,x:0,number:3,signals:[De()]},{name:"DOUT",y:40.5,x:0,number:4,signals:[]}]}render(){const{led2:e,led1:i}=this;return G`
      <svg
        width="37.056mm"
        height="13.346mm"
        clip-rule="evenodd"
        fill-rule="evenodd"
        version="1.1"
        viewBox="0 0 140 50.4"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="3.6" width="14" patternUnits="userSpaceOnUse">
            <path
              d="m5.09 1.3c0-0.618-0.502-1.12-1.12-1.12h-3.97v2.6h3.97c0.618 0 1.12-0.502 1.12-1.12z"
              fill="#eaecec"
            />
            <path
              d="m5.09 1.3c0-0.297-0.118-0.582-0.328-0.792-0.211-0.21-0.496-0.328-0.793-0.328h-1.14v2.6h1.14c0.297 0 0.582-0.118 0.793-0.328 0.21-0.21 0.328-0.495 0.328-0.793z"
              fill="#adafb0"
            />
          </pattern>
          <g id="header-pin">
            <path d="m3.4 1.8v-6.55h-6.55v6.55z" fill="#433b38" />
            <path
              d="m0 0c0.382 0 0.748-0.152 1.02-0.422 0.27-0.27 0.421-0.637 0.421-1.02s-0.151-0.748-0.421-1.02c-0.271-0.27-0.637-0.422-1.02-0.422h-26.1c-0.233 0-0.423 0.19-0.423 0.424v2.04c0 0.233 0.19 0.423 0.423 0.423h26.1z"
              fill="#9f9f9f"
            />
          </g>
        </defs>

        <!-- Board -->
        <path
          d="m133 0h-113v50.4h113zm-84.3 16.8c4.65 0 8.43 3.78 8.43 8.43 0 4.65-3.78 8.43-8.43 8.43s-8.43-3.78-8.43-8.43c0-4.65 3.78-8.43 8.43-8.43z"
          fill="#931917"
        />
        <path
          d="m48.8 13.9c6.24 0 11.3 5.07 11.3 11.3 0 6.24-5.07 11.3-11.3 11.3s-11.3-5.07-11.3-11.3c0-6.24 5.07-11.3 11.3-11.3zm0 2.88c4.65 0 8.43 3.78 8.43 8.43 0 4.65-3.78 8.43-8.43 8.43s-8.43-3.78-8.43-8.43c0-4.65 3.78-8.43 8.43-8.43z"
          fill="#d6d6d2"
        />

        <!-- Chip -->
        <rect transform="translate(87,26)" width="5" height="14.5" fill="url(#pin-pattern)" />
        <rect
          transform="translate(74,40) rotate(180)"
          width="5"
          height="14.5"
          fill="url(#pin-pattern)"
        />
        <rect x="73.5" y="25.2" width="13.9" height="15.6" fill="#3b3838" />
        <path
          d="m88.7 40.9v3.24h-6.63c0-1.02-0.721-1.85-1.61-1.85s-1.61 0.827-1.61 1.85h-6.62v-3.24"
          fill="none"
          stroke="#fff"
          stroke-width=".4px"
        />
        <path d="m72.2 25.2v-1.74h16.5v1.74" fill="none" stroke="#fff" stroke-width=".4px" />

        <!-- PCB pins -->
        <path d="m31 44.6v-38h-8.39v38z" fill="none" stroke="#fff" stroke-width=".4px" />
        <use xlink:href="#header-pin" x="26.6" y="12.4" />
        <use xlink:href="#header-pin" x="26.6" y="22.1" />
        <use xlink:href="#header-pin" x="26.6" y="31.9" />
        <use xlink:href="#header-pin" x="26.6" y="41.6" />

        <!-- Potentiometer -->
        <rect x="65.3" y="2.76" width="28.2" height="14.2" fill="#4875ce" />
        <path
          d="m94.9 2.1c0-0.221-0.179-0.4-0.4-0.4h-30.1c-0.221 0-0.4 0.179-0.4 0.4v15.5c0 0.221 0.179 0.4 0.4 0.4h30.1c0.221 0 0.4-0.179 0.4-0.4zm-0.4 0h-30.1v15.5h30.1z"
          fill="#fff"
        />
        <circle cx="69.9" cy="6.54" r="2.31" fill="#f1d99f" />
        <path
          d="m69.3 4.34c0.196-0.066 0.405-0.096 0.622-0.096 0.216 0 0.426 0.03 0.622 0.096v4.42c-0.196 0.066-0.406 0.096-0.622 0.096-0.217 0-0.426-0.03-0.622-0.096z"
          fill="#a4987a"
        />

        <!-- LED2 -->
        <rect
          x="50.5"
          y="1.4"
          width="11.5"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="51.5" y="2.12" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="54.3" y="2.12" width="3.98" height="3.28" fill="#f1d99f" />

        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="2" />
        </filter>
        ${e&&V`<circle cx="56" cy="4" r="5" fill="#80ff80" filter="url(#ledFilter)" />`}

        <!-- LED1 -->
        <rect
          x="50.5"
          y="44.4"
          width="11.5"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="51.5" y="45.1" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="54.3" y="45.1" width="3.98" height="3.28" fill="#f1d99f" />

        ${i&&V`<circle cx="56" cy="47" r="5" fill="#80ff80" filter="url(#ledFilter)" />`}

        <!-- Resistors -->
        <rect
          x="32.3"
          y="1.4"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="33.2" y="2.12" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="36" y="2.52" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="1.92"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="2.64" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="3.05" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="43.9"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="44.6" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="45" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="9"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="9.72" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="10.1" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="97.1"
          y="36.8"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="98.1" y="37.6" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="101" y="38" width="3.97" height="2.47" fill="#171514" />
        <rect
          x="32.3"
          y="44.4"
          width="11.4"
          height="4.72"
          fill="#171514"
          stroke="#fff"
          stroke-width=".4px"
        />
        <rect x="33.2" y="45.1" width="9.56" height="3.28" fill="#a19e9e" />
        <rect x="36" y="45.5" width="3.97" height="2.47" fill="#171514" />

        <!-- Texts -->
        <text fill="#ffffff" font-family="sans-serif" font-size="3.72px">
          <tspan x="33.049" y="12.085">AO</tspan>
          <tspan x="33.049" y="41.951">DO</tspan>
          <tspan x="33.049" y="22.067">G</tspan>
          <tspan x="32.964001" y="32.418999" font-size="5.25px">+</tspan>
        </text>

        <text
          transform="rotate(90 92.4 -43.2)"
          fill="#ffffff"
          font-family="sans-serif"
          font-size="3.72px"
          x="137.13"
          y="3.60"
        >
          L2
        </text>

        <text
          transform="rotate(90 92.4 -.394)"
          fill="#ffffff"
          font-family="sans-serif"
          font-size="3.72px"
          x="137.13"
          y="46.38"
        >
          L1
        </text>

        <!-- Microphone -->
        <path
          d="m110 24.7c0-1.29-1.05-2.34-2.34-2.34h-1e-3c-1.29 0-2.34 1.05-2.34 2.34v0.989c0 1.3 1.05 2.34 2.34 2.34h1e-3c1.29 0 2.34-1.05 2.34-2.34v-0.989z"
          fill="#f8f3e9"
        />
        <circle cx="108" cy="25.2" r="1.14" fill="#3b3838" />
        <path
          d="m115 6.29c1.29 0 2.34-1.05 2.34-2.34v-1e-3c0-1.29-1.05-2.34-2.34-2.34h-0.99c-1.29 0-2.34 1.05-2.34 2.34v1e-3c0 1.29 1.05 2.34 2.34 2.34h0.99z"
          fill="#f8f3e9"
        />
        <path
          d="m116 3.95c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.512-1.14-1.14c0-0.632 0.512-1.14 1.14-1.14s1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m118 27.8c0-0.622-0.247-1.22-0.686-1.66-0.44-0.44-1.04-0.687-1.66-0.687-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66v3.03c0 0.622 0.247 1.22 0.687 1.66 0.439 0.439 1.04 0.686 1.66 0.686 0.622 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66z"
          fill="#f8f3e9"
        />
        <circle cx="115" cy="29.3" r="1.14" fill="#3b3838" />
        <path
          d="m129 47.7c0.621 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66 0-0.621-0.247-1.22-0.686-1.66-0.44-0.44-1.04-0.687-1.66-0.687h-3.03c-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66 0 0.622 0.247 1.22 0.687 1.66 0.439 0.439 1.04 0.686 1.66 0.686z"
          fill="#f8f3e9"
        />
        <path
          d="m128 45.4c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.513-1.14-1.14 0.512-1.14 1.14-1.14 1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m129 6.29c0.621 0 1.22-0.247 1.66-0.687 0.439-0.439 0.686-1.04 0.686-1.66v-1e-3c0-0.621-0.247-1.22-0.686-1.66-0.44-0.439-1.04-0.686-1.66-0.686h-3.03c-0.621 0-1.22 0.247-1.66 0.686-0.44 0.44-0.687 1.04-0.687 1.66v1e-3c0 0.621 0.247 1.22 0.687 1.66 0.439 0.44 1.04 0.687 1.66 0.687z"
          fill="#f8f3e9"
        />
        <path
          d="m128 3.95c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.512-1.14-1.14c0-0.632 0.512-1.14 1.14-1.14s1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m117 41.4c0.621 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66s-0.247-1.22-0.686-1.66c-0.44-0.44-1.04-0.687-1.66-0.687h-3.03c-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66s0.247 1.22 0.687 1.66c0.439 0.439 1.04 0.686 1.66 0.686z"
          fill="#f8f3e9"
        />
        <path
          d="m114 39.1c0 0.631-0.512 1.14-1.14 1.14-0.631 0-1.14-0.513-1.14-1.14s0.512-1.14 1.14-1.14c0.632 0 1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
        <path
          d="m117 9.6c0.621 0 1.22 0.247 1.66 0.686 0.439 0.44 0.686 1.04 0.686 1.66 0 0.621-0.247 1.22-0.686 1.66-0.44 0.44-1.04 0.687-1.66 0.687h-3.03c-0.621 0-1.22-0.247-1.66-0.687-0.44-0.439-0.687-1.04-0.687-1.66 0-0.622 0.247-1.22 0.687-1.66 0.439-0.439 1.04-0.686 1.66-0.686z"
          fill="#f8f3e9"
        />
        <path
          d="m114 11.9c0-0.631-0.512-1.14-1.14-1.14-0.631 0-1.14 0.513-1.14 1.14s0.512 1.14 1.14 1.14c0.632 0 1.14-0.512 1.14-1.14z"
          fill="#3b3838"
        />
        <path
          d="m118 19.2c0-0.621-0.247-1.22-0.686-1.66-0.44-0.44-1.04-0.687-1.66-0.687-0.621 0-1.22 0.247-1.66 0.687-0.44 0.439-0.687 1.04-0.687 1.66v3.03c0 0.621 0.247 1.22 0.687 1.66 0.439 0.439 1.04 0.686 1.66 0.686 0.622 0 1.22-0.247 1.66-0.686 0.439-0.44 0.686-1.04 0.686-1.66z"
          fill="#f8f3e9"
        />
        <circle cx="115" cy="20.7" r="1.14" fill="#3b3838" />
        <path
          d="m139 9.6h-16.2c-0.391 0-0.766 0.155-1.04 0.431-0.277 0.277-0.432 0.652-0.432 1.04v28.8c0 0.391 0.155 0.766 0.432 1.04 0.276 0.277 0.651 0.432 1.04 0.432h16.2z"
          fill="#e1e0da"
        />
        <rect x="139" y="9.6" width="1.55" height="31.8" fill="#3b3838" />
        <path
          d="m121 29.3 17.7-3.26v-16.5h-16.2c-0.391 0-0.766 0.155-1.04 0.431-0.277 0.277-0.432 0.652-0.432 1.04z"
          fill="#f3f0f4"
        />
        <path
          d="m121 28.5h-4.93c-0.21 0-0.411 0.083-0.56 0.232-0.148 0.148-0.231 0.349-0.231 0.559v0.022c0 0.21 0.083 0.411 0.231 0.559 0.149 0.149 0.35 0.232 0.56 0.232h4.93z"
          fill="#afafaf"
        />
        <path
          d="m121 20h-4.93c-0.21 0-0.411 0.084-0.56 0.232-0.148 0.148-0.231 0.35-0.231 0.559v0.022c0 0.21 0.083 0.411 0.231 0.559 0.149 0.149 0.35 0.232 0.56 0.232h4.93z"
          fill="#afafaf"
        />
        <path
          d="m115 6.29c1.29 0 2.34-1.05 2.34-2.34v-1e-3c0-1.29-1.05-2.34-2.34-2.34h-0.99c-1.29 0-2.34 1.05-2.34 2.34v1e-3c0 1.29 1.05 2.34 2.34 2.34h0.99z"
          fill="#f8f3e9"
        />
        <path
          d="m116 3.95c0 0.631-0.513 1.14-1.14 1.14s-1.14-0.512-1.14-1.14c0-0.632 0.512-1.14 1.14-1.14s1.14 0.512 1.14 1.14z"
          fill="#3b3838"
        />
      </svg>
    `}};sn([y()],J1.prototype,"led1",void 0);sn([y()],J1.prototype,"led2",void 0);J1=sn([U("wokwi-big-sound-sensor")],J1);var zl=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let bs=class extends L{constructor(){super(...arguments);this.led1=!1,this.pinInfo=[{name:"INT",x:7.28,y:5.78,signals:[]},{name:"AD0",x:16.9,y:5.78,signals:[]},{name:"XCL",x:26.4,y:5.78,signals:[]},{name:"XDA",x:36,y:5.78,signals:[]},{name:"SDA",x:45.6,y:5.78,signals:[ve("SDA")]},{name:"SCL",x:55.2,y:5.78,signals:[ve("SCL")]},{name:"GND",x:64.8,y:5.78,signals:[$e()]},{name:"VCC",x:74.4,y:5.78,signals:[De()]}]}render(){const{led1:e}=this;return G`
      <svg
        width="21.6mm"
        height="16.2mm"
        clip-rule="evenodd"
        fill-rule="evenodd"
        version="1.1"
        viewBox="0 0 81.6 61.2"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="2.1" width="14" patternUnits="userSpaceOnUse">
            <path
              d=" m2.09 1.32c0.124 0 0.243-0.049 0.331-0.137 0.086-0.086 0.137-0.206 0.137-0.33v-0.387c0-0.124-0.050-0.242-0.137-0.33-0.087-0.087-0.207-0.137-0.331-0.137h-1.62v1.32z"
              fill="#f5f9f0"
            />
          </pattern>
        </defs>

        <!-- Board -->
        <path
          d="m81.6 0h-81.6v61.2h81.6zm-10 44.9c3.8 0 6.88 3.08 6.88 6.88 0 3.8-3.08 6.89-6.88 6.89-3.8 0-6.89-3.09-6.89-6.89 0-3.8 3.09-6.88 6.89-6.88zm-61.6 0c3.8 0 6.89 3.08 6.89 6.88 0 3.8-3.09 6.89-6.89 6.89-3.8 0-6.88-3.09-6.88-6.89 0-3.8 3.08-6.88 6.88-6.88zm-2.74-41.9c1.55 0 2.81 1.26 2.81 2.81s-1.26 2.8-2.81 2.8-2.8-1.26-2.8-2.8 1.26-2.81 2.8-2.81zm19.2 0c1.55 0 2.81 1.26 2.81 2.81s-1.26 2.8-2.81 2.8c-1.55 0-2.8-1.26-2.8-2.8s1.26-2.81 2.8-2.81zm-9.58 0c1.55 0 2.81 1.26 2.81 2.81s-1.26 2.8-2.81 2.8c-1.55 0-2.8-1.26-2.8-2.8s1.26-2.81 2.8-2.81zm19.2 0c1.55 0 2.81 1.26 2.81 2.81s-1.26 2.8-2.81 2.8c-1.55 0-2.8-1.26-2.8-2.8s1.26-2.81 2.8-2.81zm9.58 0c1.55 0 2.8 1.26 2.8 2.81s-1.26 2.8-2.8 2.8c-1.55 0-2.81-1.26-2.81-2.8s1.26-2.81 2.81-2.81zm19.2 0c1.55 0 2.8 1.26 2.8 2.81s-1.26 2.8-2.8 2.8-2.81-1.26-2.81-2.8 1.26-2.81 2.81-2.81zm-9.58 0c1.55 0 2.8 1.26 2.8 2.81s-1.26 2.8-2.8 2.8c-1.55 0-2.81-1.26-2.81-2.8s1.26-2.81 2.81-2.81zm19.2 0c1.55 0 2.8 1.26 2.8 2.81s-1.26 2.8-2.8 2.8c-1.55 0-2.81-1.26-2.81-2.8s1.26-2.81 2.81-2.81z"
          fill="#16619d"
        />

        <!-- Right Chip -->
        <g fill="#fefdf4">
          <rect x="74.5" y="23.1" width="2.01" height="4.81" />
          <rect x="67.8" y="33" width="2.01" height="4.81" />
          <rect x="71.2" y="23.1" width="2.01" height="4.81" />
          <rect x="67.8" y="23.1" width="2.01" height="4.81" />
          <rect x="74.5" y="33" width="2.01" height="4.81" />
        </g>
        <g fill="#31322e">
          <rect x="74.5" y="25.5" width="2.01" height="2.4" />
          <rect x="67.8" y="33" width="2.01" height="2.4" />
          <rect x="71.2" y="25.5" width="2.01" height="2.4" />
          <rect x="67.8" y="25.5" width="2.01" height="2.4" />
          <rect x="74.5" y="33" width="2.01" height="2.4" />
        </g>

        <!-- Resistors -->
        <g fill="#e5e5e5">
          <rect x="12" y="21.3" width="3.83" height="9.3" />
          <rect x="17.7" y="21.3" width="3.83" height="9.3" />
          <rect x="56.5" y="21.3" width="3.83" height="9.3" />
          <rect x="51.2" y="21.3" width="3.83" height="9.3" />
          <rect x="17.7" y="35.6" width="3.83" height="9.3" />
          <rect x="23.3" y="21.3" width="3.83" height="9.3" />
          <rect x="62.2" y="21.3" width="3.83" height="9.3" />
          <rect x="51.2" y="35.8" width="3.83" height="9.3" />
          <rect x="56.9" y="35.8" width="3.83" height="9.3" />
        </g>
        <path d="m76 42.6v-3.13h-7.59v3.13z" fill="#fefdf4" />
        <rect x="23.1" y="35.6" width="3.83" height="9.3" fill="#e5e5e5" />

        <g fill="#26232b">
          <rect x="17.7" y="23.4" width="3.83" height="5.31" />
          <rect x="56.5" y="23.4" width="3.83" height="5.31" />
          <rect x="51.2" y="23.4" width="3.83" height="5.31" />
          <rect x="17.7" y="37.7" width="3.83" height="5.31" />
        </g>
        <g fill="#d8c18d">
          <rect x="23.3" y="23.4" width="3.83" height="5.31" />
          <rect x="62.2" y="23.4" width="3.83" height="5.31" />
          <rect x="51.2" y="37.8" width="3.83" height="5.31" />
          <rect x="56.9" y="37.8" width="3.83" height="5.31" />
          <path d="m74.3 42.6v-3.13h-4.33v3.13z" />
        </g>
        <g>
          <rect x="23.1" y="37.7" width="3.83" height="5.31" fill="#a06352" />
          <rect x="31.8" y="47.1" width="15.6" height="6.03" fill="#f3c338" />
          <rect x="67.3" y="27.9" width="9.76" height="5.28" fill="#010303" />
        </g>

        <!-- MPU6050 Chip -->
        <rect transform="translate(47,26)" width="5" height="14.5" fill="url(#pin-pattern)" />
        <rect
          transform="translate(32.3,40) rotate(180)"
          width="5"
          height="14.5"
          fill="url(#pin-pattern)"
        />
        <rect
          transform="translate(46.5,40.7) rotate(90)"
          width="5"
          height="14.5"
          fill="url(#pin-pattern)"
        />
        <rect
          transform="translate(32.3,26) rotate(270)"
          width="5"
          height="14.5"
          fill="url(#pin-pattern)"
        />
        <rect x="31.8" y="25.4" width="15.6" height="15.6" />

        <!-- LED -->
        <rect x="12" y="23.4" width="3.83" height="5.31" fill="#f5ecde" />
        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="2" />
        </filter>
        ${e&&V`<circle cx="13.9" cy="25.5" r="3.5" fill="#80ff80" filter="url(#ledFilter)" />`}

        <!-- PCB Pins-->
        <g fill="none" stroke="#d0ae88" stroke-width=".648px">
          <circle cx="64.8" cy="5.78" r="2.81" />
          <circle cx="55.2" cy="5.78" r="2.81" />
          <circle cx="45.6" cy="5.78" r="2.81" />
          <circle cx="36" cy="5.78" r="2.81" />
          <circle cx="26.4" cy="5.78" r="2.81" />
          <circle cx="16.9" cy="5.78" r="2.81" />
          <circle cx="7.28" cy="5.78" r="2.81" />
          <circle cx="74.4" cy="5.78" r="2.81" />
        </g>

        <!-- Text -->
        <text
          transform="rotate(90)"
          fill="#ffffff"
          font-family="sans-serif"
          font-size="3.6px"
          x="10.056"
        >
          <tspan x="10.056" y="-6">INT</tspan>
          <tspan x="10.056" y="-15.5">AD0</tspan>
          <tspan x="10.056" y="-25.157">XCL</tspan>
          <tspan x="10.056" y="-34.5">XDA</tspan>
          <tspan x="10.056" y="-44.38">SDA</tspan>
          <tspan x="9.911" y="-54">SCL</tspan>
          <tspan x="10.057" y="-63.54">GND</tspan>
          <tspan x="10.057" y="-73">VCC</tspan>
        </text>
      </svg>
    `}};zl([y()],bs.prototype,"led1",void 0);bs=zl([U("wokwi-mpu6050")],bs);var nn=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let Q1=class extends L{constructor(){super(...arguments);this.led1=!1,this.ledPower=!1,this.pinInfo=[{name:"VIN",x:5,y:158.5,signals:[{type:"power",signal:"VCC"}]},{name:"GND.2",x:5,y:149,signals:[{type:"power",signal:"GND"}]},{name:"D13",x:5,y:139.5,signals:[te("MOSI",0),{type:"pwm"}]},{name:"D12",x:5,y:130.4,signals:[te("MISO",0),{type:"pwm"}]},{name:"D14",x:5,y:120,signals:[te("SCK",0),{type:"pwm"}]},{name:"D27",x:5,y:110.8,signals:[{type:"pwm"}]},{name:"D26",x:5,y:101,signals:[{type:"pwm"}]},{name:"D25",x:5,y:91.3,signals:[{type:"pwm"}]},{name:"D33",x:5,y:81.7,signals:[{type:"pwm"}]},{name:"D32",x:5,y:72.2,signals:[{type:"pwm"}]},{name:"D35",x:5,y:62.9,signals:[]},{name:"D34",x:5,y:53.1,signals:[]},{name:"VN",x:5,y:44,signals:[]},{name:"VP",x:5,y:34,signals:[]},{name:"EN",x:5,y:24,signals:[]},{name:"3V3",x:101.3,y:158.5,signals:[{type:"power",signal:"VCC",voltage:3.3}]},{name:"GND.1",x:101.3,y:149,signals:[{type:"power",signal:"GND"}]},{name:"D15",x:101.3,y:139.5,signals:[te("SS",0),{type:"pwm"}]},{name:"D2",x:101.3,y:130.4,signals:[{type:"pwm"}]},{name:"D4",x:101.3,y:120,signals:[{type:"pwm"}]},{name:"RX2",x:101.3,y:110.8,signals:[ze("RX",2),{type:"pwm"}]},{name:"TX2",x:101.3,y:101,signals:[ze("TX",2),{type:"pwm"}]},{name:"D5",x:101.3,y:91.3,signals:[te("SS",1),{type:"pwm"}]},{name:"D18",x:101.3,y:81.7,signals:[te("SCK",1),{type:"pwm"}]},{name:"D19",x:101.3,y:72.2,signals:[te("MISO",1),{type:"pwm"}]},{name:"D21",x:101.3,y:62.9,signals:[ve("SDA"),{type:"pwm"}]},{name:"RX0",x:101.3,y:53.1,signals:[ze("RX",0),{type:"pwm"}]},{name:"TX0",x:101.3,y:44,signals:[ze("TX",0),{type:"pwm"}]},{name:"D22",x:101.3,y:34,signals:[ve("SCL"),{type:"pwm"}]},{name:"D23",x:101.3,y:24,signals:[te("MOSI",1),{type:"pwm"}]}]}render(){const{ledPower:e,led1:i}=this;return G`
      <svg
        width="28.2mm"
        height="54.053mm"
        version="1.1"
        viewBox="0 0 107 201"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <pattern id="pin-pattern" height="4.6" width="5" patternUnits="userSpaceOnUse">
            <path
              d="m3.5 2.85c0.268 1.82e-4 0.525-0.106 0.716-0.296 0.187-0.19 0.296-0.445 0.297-0.713l5.7e-4 -0.836c1.82e-4 -0.268-0.109-0.525-0.296-0.716-0.19-0.19-0.447-0.296-0.715-0.297l-3.5-0.0024-0.0019 2.85z"
              fill="#d1c479"
              stroke-width="3.11"
            />
          </pattern>
          <pattern id="small-pin-pattern" height="4.6" width="2.5" patternUnits="userSpaceOnUse">
            <path
              d="m1.4 1.32c0-0.138-0.0547-0.271-0.153-0.37-0.098-0.0965-0.23-0.153-0.368-0.153h-0.432c-0.138 0-0.271 0.0563-0.37 0.153-0.098 0.098-0.153 0.231-0.153 0.37v1.81h1.47z"
              fill="#f5f9f0"
              stroke-width="1.61"
            />
          </pattern>
          <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
            <feGaussianBlur stdDeviation="2" />
          </filter>
        </defs>

        <!-- Board -->
        <path
          d="m7.56 0c-4.19 0-7.56 3.37-7.56 7.56v181c0 4.19 3.37 7.56 7.56 7.56h91.5c4.19 0 7.56-3.37 7.56-7.56v-181c0-4.19-3.37-7.56-7.56-7.56zm1.11 2.5a6.24 6.24 0 0 1 6.24 6.24 6.24 6.24 0 0 1-6.24 6.24 6.24 6.24 0 0 1-6.24-6.24 6.24 6.24 0 0 1 6.24-6.24zm88.9 0.217a6.24 6.24 0 0 1 6.24 6.24 6.24 6.24 0 0 1-6.24 6.24 6.24 6.24 0 0 1-6.24-6.24 6.24 6.24 0 0 1 6.24-6.24zm3.75 15.8a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.5 0.438a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.6 9.15a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.8 0.344a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.8 9.7a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.6 0.27a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.6 9.24a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.6 0.0391a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm0.0762 9.58a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.6 0.0371a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm0 9.58a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.6 0.422a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm0 9.51a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.5 0.115a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.4 9.54a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.5 0.0391a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.5 9.7a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.5 0.346a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.7 9.35a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.7 0.154a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.6 9.43a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.7 0.23a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm0 9.58a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.7 0.23a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.7 9.35a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.7 0.152a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.8 9.51a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.7 0.154a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm96.7 9.43a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm-96.7 0.152a3.4 3.4 0 0 1 3.4 3.4 3.4 3.4 0 0 1-3.4 3.4 3.4 3.4 0 0 1-3.4-3.4 3.4 3.4 0 0 1 3.4-3.4zm3.79 24.7a6.24 6.24 0 0 1 6.24 6.24 6.24 6.24 0 0 1-6.24 6.24 6.24 6.24 0 0 1-6.24-6.24 6.24 6.24 0 0 1 6.24-6.24zm88.7 0.217a6.24 6.24 0 0 1 6.24 6.24 6.24 6.24 0 0 1-6.24 6.24 6.24 6.24 0 0 1-6.24-6.24 6.24 6.24 0 0 1 6.24-6.24z"
          fill="#1a1a1a"
          fill-rule="evenodd"
        />

        <!-- ESP32 Chip -->
        <rect transform="translate(85,34)" width="4.8" height="55" fill="url(#pin-pattern)"></rect>
        <rect
          transform="translate(20.5,87) rotate(180)"
          width="4.8"
          height="55"
          fill="url(#pin-pattern)"
        ></rect>
        <rect
          transform="translate(80,98) rotate(90)"
          width="4.8"
          height="55"
          fill="url(#pin-pattern)"
        ></rect>
        <rect x="20" y="24.8" width="65.6" height="73.3" fill="#808080" fill-rule="evenodd" />

        <!-- Regulator -->
        <g fill="#ececec" fill-rule="evenodd">
          <rect x="19.3" y="143" width="7.21" height="11.5" />
          <rect x="39.8" y="139" width="6.59" height="3.07" />
          <rect x="40" y="147" width="6.59" height="3.07" />
          <rect x="40.2" y="156" width="6.59" height="3.07" />
        </g>
        <rect x="26.3" y="137" width="14" height="24.7" fill="#4d4d4d" fill-rule="evenodd" />

        <!-- Buttons -->
        <g stroke-width="1.24">
          <rect x="77.6" y="177" width="11.1" height="9.96" fill="#cecccb" />
          <circle cx="83.2" cy="182" r="3.48" fill="#ffdc8e" />
          <g fill="#cecccb">
            <path d="m80.7 190h-1.34v1.64c0 0.734 0.595 1.33 1.33 1.33h0.0099z" />
            <path d="m80.7 175h-1.34v-1.64c0-0.734 0.595-1.33 1.33-1.33h0.0099z" />
            <rect x="81" y="190" width="5.23" height="2.59" />
            <rect x="81" y="173" width="5.23" height="2.59" />
            <path
              d="m84.5 175c0.062 0 0.122 0.0248 0.166 0.0682 0.0434 0.0446 0.0682 0.104 0.0682 0.167 0 0.134 0.0533 0.263 0.149 0.358 0.0955 0.0942 0.224 0.148 0.358 0.148h0.0236c0.141 0 0.277-0.0558 0.376-0.155s0.155-0.234 0.155-0.374v-0.564h2.16v3.09h-1.69v0.744h-2.16v-0.392h-1.87v0.392h-2.16v-0.744h-1.69v-3.09h2.16v0.564c0 0.14 0.0558 0.275 0.155 0.374s0.234 0.155 0.376 0.155h0.0236c0.135 0 0.263-0.0533 0.358-0.148 0.0955-0.0955 0.149-0.224 0.149-0.358 0-0.0632 0.0248-0.123 0.0682-0.167 0.0446-0.0434 0.104-0.0682 0.167-0.0682z"
            />
            <path
              d="m81.8 190c-0.0632 0-0.123-0.0248-0.167-0.0694-0.0434-0.0434-0.0682-0.103-0.0682-0.166 0-0.134-0.0533-0.263-0.149-0.358-0.0955-0.0955-0.223-0.149-0.358-0.149h-0.0236c-0.141 0-0.277 0.0558-0.376 0.156-0.0992 0.0992-0.155 0.234-0.155 0.374v0.564h-2.16v-3.09h1.69v-0.745h2.16v0.393h1.87v-0.393h2.16v0.745h1.69v3.09h-2.16v-0.564c0-0.14-0.0558-0.275-0.155-0.374-0.0992-0.1-0.234-0.156-0.376-0.156h-0.0236c-0.134 0-0.263 0.0533-0.358 0.149s-0.149 0.224-0.149 0.358c0 0.0632-0.0248 0.123-0.0682 0.166-0.0446 0.0446-0.104 0.0694-0.166 0.0694z"
            />
          </g>
        </g>
        <g stroke-width="1.24">
          <rect x="17.7" y="177" width="11.1" height="9.96" fill="#cecccb" />
          <circle cx="23.3" cy="182" r="3.48" fill="#ffdc8e" />
          <g fill="#cecccb">
            <path d="m20.8 189h-1.34v1.64c0 0.734 0.595 1.33 1.33 1.33h0.0099z" />
            <path d="m20.8 175h-1.34v-1.64c0-0.734 0.595-1.33 1.33-1.33h0.0099z" />
            <rect x="21" y="189" width="5.23" height="2.59" />
            <rect x="21" y="172" width="5.23" height="2.59" />
            <path
              d="m24.5 175c0.062 0 0.122 0.0248 0.166 0.0682 0.0434 0.0446 0.0682 0.104 0.0682 0.167 0 0.134 0.0533 0.263 0.149 0.358 0.0955 0.0942 0.224 0.148 0.358 0.148h0.0236c0.141 0 0.277-0.0558 0.376-0.155s0.155-0.234 0.155-0.374v-0.564h2.16v3.09h-1.69v0.744h-2.16v-0.392h-1.87v0.392h-2.16v-0.744h-1.69v-3.09h2.16v0.564c0 0.14 0.0558 0.275 0.155 0.374s0.234 0.155 0.376 0.155h0.0236c0.135 0 0.263-0.0533 0.358-0.148 0.0955-0.0955 0.149-0.224 0.149-0.358 0-0.0632 0.0248-0.123 0.0682-0.167 0.0446-0.0434 0.104-0.0682 0.167-0.0682z"
            />
            <path
              d="m21.9 189c-0.0632 0-0.123-0.0248-0.167-0.0694-0.0434-0.0434-0.0682-0.103-0.0682-0.166 0-0.134-0.0533-0.263-0.149-0.358-0.0955-0.0955-0.223-0.149-0.358-0.149h-0.0236c-0.141 0-0.277 0.0558-0.376 0.156-0.0992 0.0992-0.155 0.234-0.155 0.374v0.564h-2.16v-3.09h1.69v-0.745h2.16v0.393h1.87v-0.393h2.16v0.745h1.69v3.09h-2.16v-0.564c0-0.14-0.0558-0.275-0.155-0.374-0.0992-0.1-0.234-0.156-0.376-0.156h-0.0236c-0.134 0-0.263 0.0533-0.358 0.149s-0.149 0.224-0.149 0.358c0 0.0632-0.0248 0.123-0.0682 0.166-0.0446 0.0446-0.104 0.0694-0.166 0.0694z"
            />
          </g>
        </g>

        <!-- USB Connection -->
        <path
          d="m66.4 197 1.06 2.24c0.0651 0.142 0.0731 0.302 0.0207 0.448-0.0525 0.147-0.16 0.266-0.301 0.332-0.14 0.0665-0.302 0.0744-0.448 0.022-0.146-0.0525-0.266-0.16-0.332-0.301l-0.724-1.54-3e-3 0.207c-6e-3 0.488-0.206 0.955-0.556 1.3-0.35 0.342-0.821 0.529-1.31 0.522l-22.2-0.29c-0.489-6e-3 -0.955-0.206-1.3-0.556-0.341-0.35-0.529-0.821-0.522-1.31l3e-3 -0.207-0.764 1.52c-0.0701 0.14-0.192 0.244-0.34 0.292-0.147 0.0486-0.307 0.0365-0.446-0.0336l-1e-3 -1e-5c-0.138-0.0701-0.244-0.192-0.292-0.34-0.0486-0.147-0.0365-0.307 0.0336-0.447l1.11-2.21-0.602-8e-3 0.269-20.6 28.2 0.369-0.269 20.6z"
          fill="#cecccb"
          stroke-width="1.26"
        />
        <path
          d="m60.7 177-0.0236 1.8c-7.9e-4 0.0607 0.0301 0.116 0.0802 0.148 0.522 0.329 3.38 2.12 3.38 2.12l-0.0217 1.66-1.74-0.0227-0.0156 1.19-2.63-0.0344 0.0156-1.19-1.66-0.0217 0.0413-3.16c2e-3 -0.136-0.0496-0.265-0.143-0.361-0.0948-0.096-0.223-0.151-0.357-0.152l-1.58-0.0207-0.0172 1.31-6.46-0.0845 0.0172-1.31-1.58-0.0207c-0.134-2e-3 -0.264 0.0496-0.36 0.143-0.0973 0.0936-0.152 0.221-0.154 0.357l-0.0413 3.16-1.66-0.0217-0.0156 1.19-2.63-0.0344 0.0156-1.19-1.74-0.0228 0.0217-1.66s2.91-1.73 3.43-2.03c0.0522-0.0309 0.0833-0.0848 0.0841-0.146l0.0236-1.8z"
          fill="#989898"
          stroke-width="1.26"
        />

        <!-- LEDs -->
        <g stroke-width="1.44">
          <rect x="35" y="108" width="3.83" height="9.3" fill="#e5e5e5" />
          <rect x="35" y="111" width="3.83" height="5.31" fill="#f5ecde" />
          ${e&&V`<circle cx="37" cy="112.5" r="4" fill="red" filter="url(#ledFilter)" />`}

          <rect x="69.5" y="108" width="3.83" height="9.3" fill="#e5e5e5" />
          <rect x="69.5" y="110" width="3.83" height="5.31" fill="#f5ecde" />
          ${i&&V`<circle cx="71.5" cy="112.5" r="4" fill="blue" filter="url(#ledFilter)" />`}
        </g>

        <!-- Small Chip-->
        <rect
          transform="translate(69,137)"
          width="13.9"
          height="3"
          fill="url(#small-pin-pattern)"
        ></rect>
        <rect
          transform="translate(82.8,160.5) rotate(180)"
          width="13.9"
          height="3"
          fill="url(#small-pin-pattern)"
        ></rect>
        <rect
          transform="translate(87.2,142) rotate(90)"
          width="13.9"
          height="3"
          fill="url(#small-pin-pattern)"
        ></rect>
        <rect
          transform="translate(64,155.8) rotate(270)"
          width="13.9"
          height="3"
          fill="url(#small-pin-pattern)"
        ></rect>
        <rect x="66.9" y="140" width="17.4" height="17.4" fill="#333" stroke-width="1.61" />

        <!-- Texts -->
        <text fill="#ffffff" font-family="sans-serif" font-size="3.72px" transform="rotate(270)">
          <tspan x="-162.21" y="12.285">VIN</tspan>
          <tspan x="-153.37" y="12.317">GND</tspan>
          <tspan x="-143.03" y="12.269">D13</tspan>
          <tspan x="-132.81" y="12.130">D12</tspan>
          <tspan x="-123.10" y="12.514">D14</tspan>
          <tspan x="-113.82" y="12.481">D27</tspan>
          <tspan x="-103.55" y="12.580">D26</tspan>
          <tspan x="-94.204" y="12.509">D25</tspan>
          <tspan x="-84.482" y="12.632">D33</tspan>
          <tspan x="-74.139" y="12.294">D32</tspan>
          <tspan x="-64.263" y="12.750">D35</tspan>
          <tspan x="-54.385" y="12.631">D34</tspan>
          <tspan x="-44.529" y="12.468">VN</tspan>
          <tspan x="-35.205" y="12.546">VP</tspan>
          <tspan x="-25.439" y="12.846">EN</tspan>
          <tspan x="-163.01" y="95.712">3V3</tspan>
          <tspan x="-153.64" y="95.392">GND</tspan>
          <tspan x="-142.86" y="95.431">D15</tspan>
          <tspan x="-131.36" y="95.296">D2</tspan>
          <tspan x="-122.53" y="95.505">D4</tspan>
          <tspan x="-114.75" y="95.613">RX2</tspan>
          <tspan x="-104.84" y="95.442">TX2</tspan>
          <tspan x="-93.899" y="95.430">D5</tspan>
          <tspan x="-85.460" y="95.585">D18</tspan>
          <tspan x="-75.415" y="95.747">D19</tspan>
          <tspan x="-65.796" y="95.687">D21</tspan>
          <tspan x="-55.802" y="95.818">RX0</tspan>
          <tspan x="-45.850" y="95.613">TX0</tspan>
          <tspan x="-36.582" y="96.012">D22</tspan>
          <tspan x="-26.250" y="95.903">D23</tspan>
        </text>
        <text x="30" y="59" fill="#cecccb" font-family="sans-serif" font-size="15px">ESP32</text>

        <!-- Antenna -->
        <path
          d="m24.3 22.1v-18.8h8v11.5h10.2v-11h8.5v10.5h10v-10.5h17.8v20.2"
          fill="none"
          stroke="#4f4c48"
          stroke-width="1px"
        />
        <path d="m69.7 4.16v19.5" fill="none" stroke="#4f4c48" stroke-width="1px" />
      </svg>
    `}};nn([y()],Q1.prototype,"led1",void 0);nn([y()],Q1.prototype,"ledPower",void 0);Q1=nn([U("wokwi-esp32-devkit-v1")],Q1);var _i=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let u1=class extends L{constructor(){super(...arguments);this.angle=0,this.stepSize=18,this.pressed=!1,this.arrowTimer=null,this.pinInfo=[{name:"CLK",y:7.9,x:116,number:1,signals:[]},{name:"DT",y:17.4,x:116,number:2,signals:[]},{name:"SW",y:27,x:116,number:3,signals:[]},{name:"VCC",y:36.3,x:116,number:4,signals:[De()]},{name:"GND",y:45.5,x:116,number:5,signals:[$e()]}]}static get styles(){return ke`
      svg {
        user-select: none;
      }

      .arrow-container {
        cursor: pointer;
      }

      svg:hover .arrow {
        fill: #666;
        stroke: #666;
        stroke-width: 1.5px;
        transition: stroke-width 0.3s;
      }

      .arrow-container:hover .arrow {
        fill: white;
      }

      svg:hover .handle:hover {
        stroke: #ccc;
        stroke-width: 1.5px;
        transition: stroke-width 0.3s;
      }

      svg:hover .handle.active {
        fill: white;
        stroke: white;
        stroke-width: 1.5px;
        transition: stroke-width 0.3s;
      }

      .handle {
        cursor: pointer;
      }

      g[tabindex]:focus {
        outline: none;
      }

      g[tabindex]:focus + .focus-indicator {
        opacity: 1;
      }
    `}render(){return G`
      <svg
        width="30.815mm"
        height="18.63mm"
        version="1.1"
        viewBox="0 0 116 70.4"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <linearGradient
            id="a"
            x1="158"
            x2="170"
            y1="86.5"
            y2="86.5"
            gradientTransform="translate(-75.1 -60.1)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#4d4d4d" offset="0" />
            <stop stop-color="#4d4d4d" stop-opacity="0" offset="1" />
          </linearGradient>
        </defs>

        <!-- Board -->
        <path
          d="m0 0v70.4h99v-70.4zm18 56.5a6.5 6.5 0 0 1 6.5 6.5 6.5 6.5 0 0 1-6.5 6.5 6.5 6.5 0 0 1-6.5-6.5 6.5 6.5 0 0 1 6.5-6.5zm63.8 0.213a6.5 6.5 0 0 1 6.5 6.5 6.5 6.5 0 0 1-6.5 6.5 6.5 6.5 0 0 1-6.5-6.5 6.5 6.5 0 0 1 6.5-6.5z"
          fill="#1a1a1a"
          fill-rule="evenodd"
        />

        <!-- Rotator -->
        <g fill="#ccc" fill-rule="evenodd">
          <rect x="9.05" y="17.4" width="6.95" height="2.47" rx=".756" />
          <rect x="9.15" y="26.5" width="6.95" height="2.47" rx=".756" />
          <rect x="9.05" y="36.1" width="6.95" height="2.47" rx=".756" />
        </g>

        <g tabindex="0" @keydown=${this.keydown} @keyup=${this.keyup}>
          <rect x="12.2" y="8.05" width="48.4" height="41" rx="7.12" fill="#e6e6e6" />

          <circle cx="36.6" cy="28.5" r="13.5" fill="#666" />
          <rect x="32.5" y="7.87" width="7.42" height="41.5" fill="#666" />

          <!-- Handle -->
          <path
            transform="rotate(${this.angle}, 36.244, 28.5)"
            d="m36.3 21.4a7.03 7.14 0 0
              0-3.74 1.1v12.1a7.03 7.14 0 0 0 3.74 1.1 7.03 7.14 0 0 0 7.03-7.14 7.03 7.14 0 0
              0-7.03-7.14z"
            fill="#ccc"
            stroke="#060606"
            stroke-width=".3"
            class="handle ${this.pressed?"active":""}"
            @mousedown=${this.press}
            @mouseup=${this.release}
            @mouseleave=${this.release}
          />

          <!-- Counter Clockwise Arrow -->
          <g
            class="arrow-container"
            @click=${this.counterClockwiseStep}
            @mousedown=${this.counterclockwiseArrowPress}
            @mouseup=${this.arrowRelease}
            @mouseleave=${this.arrowRelease}
          >
            <circle cx="20" cy="43" r="12" fill="red" opacity="0" />
            <path
              d="m21 44.5c-5.17-1.78-7.55-5.53-6.6-11.2 0.0662-0.327 0.107-0.938 0.272-1.06 0.204-0.137 0.312-0.116 0.39-0.1 0.0775 0.0152 0.139 0.0274 0.189 0.102 0.846 3.81 3.13 6.84 6.57 7.59 0.304-0.787 0.461-3.32 0.826-3.24 0.428 0.0848 4.31 5.73 4.93 6.65-0.978 0.839-6.07 4.44-6.95 4.28 0 0 0.206-2.19 0.362-2.96z"
              fill="#b3b3b3"
              stroke="#000"
              stroke-width=".0625px"
              class="arrow"
            />
          </g>

          <!-- Clockwise Arrow -->
          <g
            class="arrow-container"
            @click=${this.clockwiseStep}
            @mousedown=${this.clockwiseArrowPress}
            @mouseup=${this.arrowRelease}
            @mouseleave=${this.arrowRelease}
          >
            <circle cx="20" cy="15" r="12" fill="red" opacity="0" />
            <path
              d="m21.2 12.1c-5.17 1.78-7.55 5.53-6.6 11.2 0.0662 0.327 0.107 0.938 0.272 1.06 0.204 0.137 0.312 0.116 0.39 0.1 0.0775-0.0152 0.139-0.0274 0.189-0.102 0.846-3.81 3.13-6.84 6.57-7.59 0.304 0.787 0.461 3.32 0.826 3.24 0.428-0.0848 4.31-5.73 4.93-6.65-0.978-0.839-6.07-4.44-6.95-4.28 0 0 0.206 2.19 0.362 2.96z"
              fill="#b3b3b3"
              stroke="#022"
              stroke-width=".0625px"
              class="arrow"
            />
          </g>
        </g>

        <!-- Focus indicator for the group above-->
        <rect
          class="focus-indicator"
          x="10.2"
          y="6.05"
          width="52.4"
          height="45"
          rx="7.12"
          stroke="white"
          stroke-width="2"
          fill="none"
          opacity="0"
        />

        <!-- Chip Pins -->
        <rect
          x="83"
          y="1.72"
          width="10.9"
          height="49.2"
          fill="url(#a)"
          fill-rule="evenodd"
          opacity=".65"
          stroke="#fff"
          stroke-width="1.16"
        />
        <g fill="#ccc" fill-rule="evenodd">
          <rect x="86.9" y="6.54" width="28.9" height="2.47" rx=".877" />
          <rect x="86.8" y="15.9" width="28.9" height="2.47" rx=".877" />
          <rect x="87.1" y="25.6" width="28.9" height="2.47" rx=".877" />
          <rect x="87.1" y="34.9" width="28.9" height="2.47" rx=".877" />
          <rect x="87.6" y="44.1" width="28.9" height="2.47" rx=".877" />
        </g>
        <g fill="#ffffff" font-family="sans-serif" letter-spacing="0px" word-spacing="0px">
          <text x="65.55" y="12.13" font-size="7.29px" fill="#ffffff" stroke-width=".182">CLK</text>
          <text x="65.02" y="21.93" font-size="7.44px" fill="#ffffff">DT</text>
          <text x="65.29" y="31.26" font-size="7.54px" fill="#ffffff">SW</text>
          <text x="70.42" y="39.99" font-size="6.82px" fill="#ffffff">+</text>
          <text x="64.31" y="49.74" font-size="7.59px" fill="#ffffff">GND</text>
        </g>
      </svg>
    `}clockwiseStep(){this.angle=(this.angle+this.stepSize)%360,this.dispatchEvent(new InputEvent("rotate-cw"))}counterClockwiseStep(){this.angle=(this.angle-this.stepSize+360)%360,this.dispatchEvent(new InputEvent("rotate-ccw"))}press(){this.pressed||(this.dispatchEvent(new InputEvent("button-press")),this.pressed=!0)}release(){this.pressed&&(this.dispatchEvent(new InputEvent("button-release")),this.pressed=!1)}counterclockwiseArrowPress(){this.arrowTimer=setInterval(()=>{this.counterClockwiseStep()},300)}clockwiseArrowPress(){this.arrowTimer=setInterval(()=>{this.clockwiseStep()},300)}arrowRelease(){this.arrowTimer!=null&&(clearInterval(this.arrowTimer),this.arrowTimer=null)}keydown(e){switch(e.key){case"ArrowUp":case"ArrowRight":this.clockwiseStep(),e.preventDefault();break;case"ArrowDown":case"ArrowLeft":this.counterClockwiseStep(),e.preventDefault();break;case" ":this.press(),e.preventDefault();break}}keyup(e){switch(e.key){case" ":e.preventDefault(),this.release();break}}};_i([y()],u1.prototype,"angle",void 0);_i([y()],u1.prototype,"stepSize",void 0);_i([y()],u1.prototype,"pressed",void 0);u1=_i([U("wokwi-ky-040")],u1);var rn=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let ei=class extends L{constructor(){super(...arguments);this.ledDO=!1,this.ledPower=!1,this.pinInfo=[{name:"VCC",x:172,y:16,signals:[De()]},{name:"GND",x:172,y:26,signals:[$e()]},{name:"DO",x:172,y:35.8,signals:[]},{name:"AO",x:172,y:45.5,signals:[H(0)]}]}render(){const{ledPower:e,ledDO:i}=this;return G`
      <svg
        width="45.95mm"
        height="16.267mm"
        version="1.1"
        viewBox="0 0 174 61.5"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <!-- board -->
        <path
          d="m153 0h-136v61.5h136zm-129 52c1.9 0 3.44 1.5 3.44 3.34s-1.54 3.34-3.44 3.34-3.44-1.5-3.44-3.34 1.54-3.34 3.44-3.34zm98.3-29.8c4.17 0 7.55 3.38 7.55 7.55 0 4.17-3.38 7.55-7.55 7.55s-7.55-3.38-7.55-7.55c0-4.17 3.38-7.55 7.55-7.55zm-98.3-19.4c1.9 0 3.44 1.5 3.44 3.34 0 1.84-1.54 3.34-3.44 3.34s-3.44-1.5-3.44-3.34c0-1.84 1.54-3.34 3.44-3.34z"
          fill="#1c2546"
        />

        <!--Photo sensor-->
        <rect
          x="18.9"
          y="20.1"
          width="11.2"
          height="22.2"
          fill="none"
          stroke="#fff"
          stroke-width="1.08px"
        />
        <circle cx="24.5" cy="25.6" r="3.14" fill="#dae3eb" />
        <circle cx="24.5" cy="36.8" r="3.14" fill="#dae3eb" />
        <path
          d="m24.5 25.7c0-0.199-0.079-0.39-0.22-0.53-0.14-0.141-0.331-0.22-0.529-0.22h-23c-0.199 0-0.389 0.079-0.53 0.22-0.14 0.14-0.219 0.331-0.219 0.53 0 0.198 0.079 0.389 0.219 0.53 0.141 0.14 0.331 0.219 0.53 0.219h23c0.198 0 0.389-0.079 0.529-0.219 0.141-0.141 0.22-0.332 0.22-0.53z"
          fill="#a8b6ba"
        />
        <path
          d="m24.5 36.7c0-0.198-0.079-0.389-0.22-0.53-0.14-0.14-0.331-0.219-0.529-0.219h-23c-0.199 0-0.389 0.079-0.53 0.219-0.14 0.141-0.219 0.332-0.219 0.53 0 0.199 0.079 0.39 0.219 0.53 0.141 0.141 0.331 0.22 0.53 0.22h23c0.198 0 0.389-0.079 0.529-0.22 0.141-0.14 0.22-0.331 0.22-0.53z"
          fill="#a8b6ba"
        />
        <path
          d="m8.64 22.8c0-0.375-0.304-0.679-0.679-0.679h-6.14c-0.375 0-0.679 0.304-0.679 0.679v16.8c0 0.375 0.304 0.679 0.679 0.679h6.14c0.375 0 0.679-0.304 0.679-0.679v-16.8z"
          fill="#cc4247"
        />
        <clipPath>
          <path
            d="m27.9 29c0-0.375-0.304-0.679-0.679-0.679h-6.14c-0.375 0-0.679 0.304-0.679 0.679v16.8c0 0.375 0.304 0.679 0.679 0.679h6.14c0.375 0 0.679-0.304 0.679-0.679v-16.8z"
          />
        </clipPath>

        <!-- Holes -->
        <g fill="none" stroke-width="1.08px">
          <ellipse cx="24.5" cy="6.11" rx="3.43" ry="3.34" stroke="#a8b6ba" />
          <ellipse cx="24.5" cy="55.4" rx="3.43" ry="3.34" stroke="#a8b6ba" />

          <!-- +/- -->
          <g stroke="#fff">
            <path d="m24 44.7v4.75" />
            <path d="m24 12.1v4.75" />
            <path d="m26.4 14.5h-4.75" />
          </g>
        </g>

        <!-- Resistors -->
        <g fill="#dae3eb">
          <rect x="37.7" y="8.69" width="16.7" height="5.52" />
          <rect x="37.7" y="22" width="16.7" height="5.52" />
          <rect x="37.7" y="34.5" width="16.7" height="5.52" />
        </g>
        <rect x="41.9" y="34.3" width="8.43" height="5.9" fill="#29261c" />
        <path d="m108 21.2v-16.7h-5.52v16.7z" fill="#dae3eb" />
        <path d="m108 17v-8.43h-5.9v8.43z" fill="#29261c" />
        <path d="m108 53.8v-16.7h-5.52v16.7z" fill="#dae3eb" />
        <path d="m108 49.7v-8.43h-5.9v8.43z" fill="#29261c" />
        <rect x="37.7" y="47.5" width="16.7" height="5.52" fill="#dae3eb" />
        <rect x="41.9" y="8.5" width="8.43" height="5.9" fill="#907463" />
        <rect x="41.9" y="21.8" width="8.43" height="5.9" fill="#907463" />
        <rect x="41.9" y="47.3" width="8.43" height="5.9" fill="#29261c" />

        <!-- LEDs -->
        <rect x="118" y="4.77" width="13" height="4.29" fill="#dae3eb" />
        <rect x="121" y="4.62" width="6.55" height="4.59" fill="#fffefe" />
        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="2" />
        </filter>
        ${e&&V`<circle cx="124.5" cy="7" r="4" fill="green" filter="url(#ledFilter)" />`}

        <rect x="118" y="52.6" width="13" height="4.29" fill="#dae3eb" />
        <rect x="121" y="52.5" width="6.55" height="4.59" fill="#fffefe" />
        ${i&&V`<circle cx="124.5" cy="55" r="4" fill="red" filter="url(#ledFilter)" />`}

        <!-- Chip -->
        <g fill="#dae3eb">
          <path
            d="m72.7 34.6h-9.67c-0.407 0-0.796 0.162-1.08 0.449-0.287 0.287-0.448 0.677-0.448 1.08v1e-3c0 0.406 0.161 0.796 0.448 1.08 0.288 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m72.7 40.4h-9.67c-0.407 0-0.796 0.162-1.08 0.449-0.287 0.287-0.448 0.677-0.448 1.08v1e-3c0 0.406 0.161 0.796 0.448 1.08 0.288 0.287 0.677 0.448 1.08 0.448h9.67z"
          />
          <path
            d="m72.7 46.2h-9.67c-0.407 0-0.796 0.162-1.08 0.449-0.287 0.287-0.448 0.677-0.448 1.08v1e-3c0 0.406 0.161 0.796 0.448 1.08 0.288 0.288 0.677 0.449 1.08 0.449h9.67z"
          />
          <path
            d="m72.7 52h-9.67c-0.407 0-0.796 0.162-1.08 0.449-0.287 0.287-0.448 0.677-0.448 1.08v1e-3c0 0.406 0.161 0.796 0.448 1.08 0.288 0.288 0.677 0.449 1.08 0.449h9.67z"
          />
          <path
            d="m84.4 55.1h9.67c0.406 0 0.796-0.161 1.08-0.449 0.288-0.287 0.449-0.677 0.449-1.08v-1e-3c0-0.406-0.161-0.796-0.449-1.08-0.287-0.287-0.677-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m84.4 49.3h9.67c0.406 0 0.796-0.161 1.08-0.449 0.288-0.287 0.449-0.677 0.449-1.08v-1e-3c0-0.406-0.161-0.796-0.449-1.08-0.287-0.287-0.677-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m84.4 43.5h9.67c0.406 0 0.796-0.161 1.08-0.448 0.288-0.288 0.449-0.678 0.449-1.08v-1e-3c0-0.406-0.161-0.796-0.449-1.08-0.287-0.287-0.677-0.449-1.08-0.449h-9.67z"
          />
          <path
            d="m84.4 37.7h9.67c0.406 0 0.796-0.161 1.08-0.448 0.288-0.288 0.449-0.678 0.449-1.08v-1e-3c0-0.406-0.161-0.796-0.449-1.08-0.287-0.287-0.677-0.449-1.08-0.449h-9.67z"
          />
        </g>
        <rect x="70.3" y="33.2" width="16.1" height="23.3" fill="#29261c" />

        <!-- Decorations -->
        <rect x="62.8" y="7.63e-9" width="32.1" height="32.1" fill="#466fb5" />
        <circle cx="78.9" cy="16" r="6.56" fill="#bcc2d5" />
        <path d="m78.9 6.72v18.6" fill="none" stroke="#3f3c40" stroke-width="2.5px" />
        <path d="m88.2 16h-18.6" fill="none" stroke="#3f3c40" stroke-width="2.5px" />
        <path
          d="m123 19.8c5.5 0 9.96 4.46 9.96 9.96s-4.46 9.96-9.96 9.96-9.96-4.46-9.96-9.96 4.46-9.96 9.96-9.96zm0 2.4c4.17 0 7.55 3.38 7.55 7.55 0 4.17-3.38 7.55-7.55 7.55s-7.55-3.38-7.55-7.55c0-4.17 3.38-7.55 7.55-7.55z"
          fill="#d4d0d1"
        />

        <!-- Text -->
        <g fill="#fffefe" font-size="4.4px" font-family="sans-serif">
          <text x="117.46" y="13.90">PWR</text>
          <text x="117.46" y="18.41">LED</text>
          <text x="133.16" y="17.37">VCC</text>
          <text x="133.16" y="26.87">GND</text>
          <text x="135.42" y="36.55">DO</text>
          <text x="135.42" y="46.359">AO</text>
          <text x="117.44" y="45.53">DO</text>
          <text x="117.44" y="50.036">LED</text>
        </g>

        <!-- Board pins -->
        <path
          d="m143 11.7v38h8.39v-38z"
          fill="none"
          stroke="#fff"
          stroke-linejoin="round"
          stroke-width=".4px"
        />
        <g fill="#433b38">
          <path d="m144 42.1v6.55h6.55v-6.55z" />
          <path d="m144 32.3v6.55h6.55v-6.55z" />
          <path d="m144 22.6v6.55h6.55v-6.55z" />
          <path d="m144 12.9v6.55h6.55v-6.55z" />
        </g>
        <g fill="#9f9f9f">
          <path
            d="m147 43.9c-0.382 0-0.748 0.152-1.02 0.422-0.27 0.27-0.421 0.636-0.421 1.02v1e-3c0 0.382 0.151 0.748 0.421 1.02 0.271 0.271 0.637 0.422 1.02 0.422h26.1c0.233 0 0.423-0.189 0.423-0.423v-2.04c0-0.234-0.19-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m147 34.2c-0.382 0-0.748 0.152-1.02 0.422-0.27 0.27-0.421 0.636-0.421 1.02v1e-3c0 0.382 0.151 0.748 0.421 1.02 0.271 0.271 0.637 0.422 1.02 0.422h26.1c0.233 0 0.423-0.189 0.423-0.423v-2.04c0-0.234-0.19-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m147 24.4c-0.382 0-0.748 0.151-1.02 0.422-0.27 0.27-0.421 0.636-0.421 1.02v1e-3c0 0.382 0.151 0.748 0.421 1.02 0.271 0.27 0.637 0.422 1.02 0.422h26.1c0.233 0 0.423-0.19 0.423-0.423v-2.04c0-0.234-0.19-0.423-0.423-0.423h-26.1z"
          />
          <path
            d="m147 14.7c-0.382 0-0.748 0.152-1.02 0.422-0.27 0.27-0.421 0.637-0.421 1.02s0.151 0.749 0.421 1.02c0.271 0.27 0.637 0.422 1.02 0.422h26.1c0.233 0 0.423-0.19 0.423-0.424v-2.03c0-0.234-0.19-0.424-0.423-0.424h-26.1z"
          />
        </g>
      </svg>
    `}};rn([y()],ei.prototype,"ledDO",void 0);rn([y()],ei.prototype,"ledPower",void 0);ei=rn([U("wokwi-photoresistor-sensor")],ei);var Ci=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let m1=class extends L{constructor(){super(...arguments);this.ledRed=0,this.ledGreen=0,this.ledBlue=0,this.pinInfo=[{name:"R",x:8.5,y:44,signals:[]},{name:"COM",x:17.5,y:54,signals:[]},{name:"G",x:26.4,y:48,signals:[]},{name:"B",x:35.7,y:43,signals:[]}]}render(){const{ledRed:e,ledGreen:i,ledBlue:s}=this,n=Math.max(e,i,s),r=n?.2+n*.6:0;return G`
      <?xml version="1.0" encoding="UTF-8"?>
      <svg
        width="42.129"
        height="72.582"
        version="1.2"
        viewBox="-17 -10 37.3425 57.5115"
        xmlns="http://www.w3.org/2000/svg"
      >
        <!-- LED Legs -->
        <g fill="none" stroke="#9D9999" stroke-linecap="round" stroke-width=".89px">
          <path d="m3.3863 15.334 3.0611 13.971" />
          <path d="m7.2753 13.972 5.9987 4.0518 1.1777 6.5679" stroke-linejoin="round" />
          <path d="m-4.3793 14.184-5.0755 5.6592-0.10206 6.1694" stroke-linejoin="round" />
          <path d="m-1.1952 15.607-0.33725 19.49" />
        </g>

        <!-- LED Body -->
        <path
          d="m8.3435 5.65v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
          opacity=".3"
        />
        <path
          d="m8.3435 5.65v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
          fill="#e6e6e6"
          opacity=".5"
        />
        <path
          d="m8.3435 5.65v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v4.6296c1.4738 1.6517 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586l-4e-5 -1.5235c-7e-4 -1.1419-0.4744-2.2032-1.283-3.1054z"
          fill="#d1d1d1"
          opacity=".9"
        />
        <g transform="translate(-5.8295 -7.351)">
          <path
            d="m14.173 13.001v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v4.6296c1.4738 1.6517 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586l-4e-5 -1.5235c-7e-4 -1.1419-0.4744-2.2032-1.283-3.1054z"
            opacity=".7"
          />
          <path
            d="m14.173 13.001v3.1054c0 2.7389-3.1658 4.9651-7.0855 4.9651-3.9125 2e-5 -7.0877-2.219-7.0877-4.9651v3.1054c1.4738 1.6502 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8586-7.4e-4 -1.1412-0.47444-2.2025-1.283-3.1047z"
            opacity=".25"
          />
          <ellipse cx="7.0877" cy="16.106" rx="7.087" ry="4.9608" opacity=".25" />
        </g>
        <polygon
          transform="translate(-5.8295 -7.351)"
          points="3.1961 13.095 6.0156 13.095 10.012 8.8049 3.407 8.8049 2.2032 9.648 2.2032 16.107 3.1961 16.107"
          fill="#666"
        />
        <polygon
          transform="translate(-5.8295 -7.351)"
          points="11.06 13.095 11.06 16.107 11.974 16.107 11.974 8.5241 10.778 8.5241 11.215 9.0338 7.4117 13.095"
          fill="#666"
        />
        <path
          d="m8.3435 5.65v-5.9126c0-3.9132-3.168-7.0884-7.0855-7.0884-3.9125 0-7.0877 3.1694-7.0877 7.0884v13.649c1.4738 1.651 4.0968 2.7526 7.0877 2.7526 4.6195 0 8.3686-2.6179 8.3686-5.8594v-1.5235c-7.4e-4 -1.1426-0.47444-2.2039-1.283-3.1061z"
          fill="white"
          opacity=".65"
        />
        <g transform="translate(-5.8295 -7.351)" fill="#fff">
          <path
            d="m10.388 3.7541 1.4364-0.2736c-0.84168-1.1318-2.0822-1.9577-3.5417-2.2385l0.25416 1.0807c0.76388 0.27072 1.4068 0.78048 1.8511 1.4314z"
            opacity=".5"
          />
          <path
            d="m0.76824 19.926v1.5199c0.64872 0.5292 1.4335 0.97632 2.3076 1.3169v-1.525c-0.8784-0.33624-1.6567-0.78194-2.3076-1.3118z"
            opacity=".5"
          />
          <path
            d="m11.073 20.21c-0.2556 0.1224-0.52992 0.22968-0.80568 0.32976-0.05832 0.01944-0.11736 0.04032-0.17784 0.05832-0.56376 0.17928-1.1614 0.31896-1.795 0.39456-0.07488 0.0094-0.1512 0.01872-0.22464 0.01944-0.3204 0.03024-0.64368 0.05832-0.97056 0.05832-0.14832 0-0.30744-0.01512-0.4716-0.02376-1.2002-0.05688-2.3306-0.31464-3.2976-0.73944l-2e-5 -8.3895v-4.8254c0-1.471 0.84816-2.7295 2.0736-3.3494l-0.02232-0.05328-1.2478-1.512c-1.6697 1.003-2.79 2.8224-2.79 4.9118v11.905c-0.04968-0.04968-0.30816-0.30888-0.48024-0.52992l-0.30744 0.6876c1.4011 1.4818 3.8088 2.4617 6.5426 2.4617 1.6798 0 3.2371-0.37368 4.5115-1.0022l-0.52704-0.40896-0.01006 0.0072z"
            opacity=".5"
          />
        </g>

        <filter id="ledFilter" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="4" />
        </filter>
        <filter id="ledFilterRed" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="${e*3}" />
        </filter>
        <filter id="ledFilterGreen" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="${i*3}" />
        </filter>
        <filter id="ledFilterBlue" x="-0.8" y="-0.8" height="5.2" width="5.8">
          <feGaussianBlur stdDeviation="${s*3}" />
        </filter>

        <circle
          cx="1.7"
          cy="3"
          r="${e*5+2}"
          fill="rgb(255, 0, 0)"
          opacity="${Math.min(e*20,.3)}"
          filter="url(#ledFilterRed)"
        />
        <circle
          cx="2.7"
          cy="5"
          r="${i*5+2}"
          fill="rgb(0, 255, 0)"
          opacity="${Math.min(i*20,.3)}"
          filter="url(#ledFilterGreen)"
        />
        <circle
          cx="0.7"
          cy="5"
          r="${s*5+2}"
          fill="rgb(1,85,253)"
          opacity="${Math.min(s*20,.3)}"
          filter="url(#ledFilterBlue)"
        />

        <circle
          cx="1.7"
          cy="4"
          r="10"
          fill="rgb(${e*255}, ${i*255+s*90}, ${s*255})"
          filter="url(#ledFilter)"
          opacity="${r}"
        />

        <!-- Grey hollow around the LED -->
        <circle
          cx="1.7"
          cy="4"
          r="13"
          stroke="#666"
          stroke-width="1"
          fill="none"
          filter="url(#ledFilter)"
          opacity="${r}"
        />
      </svg>
    `}};Ci([y()],m1.prototype,"ledRed",void 0);Ci([y()],m1.prototype,"ledGreen",void 0);Ci([y()],m1.prototype,"ledBlue",void 0);m1=Ci([U("wokwi-rgb-led")],m1);var ln=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let ti=class extends L{constructor(){super(...arguments);this.screenWidth=240,this.screenHeight=320,this.flipHorizontal=!1,this.flipVertical=!1,this.pinInfo=[{name:"VCC",x:48.3,y:287.2,signals:[{type:"power",signal:"VCC"}]},{name:"GND",x:57.9012,y:287.2,signals:[{type:"power",signal:"GND"}]},{name:"CS",x:67.5024,y:287.2,signals:[te("SS")]},{name:"RST",x:77.1036,y:287.2,signals:[]},{name:"D/C",x:86.7048,y:287.2,signals:[]},{name:"MOSI",x:96.306,y:287.2,signals:[te("MOSI")]},{name:"SCK",x:105.9072,y:287.2,signals:[te("SCK")]},{name:"LED",x:115.5084,y:287.2,signals:[]},{name:"MISO",x:125.1096,y:287.2,signals:[te("MISO")]}]}static get styles(){return ke`
      .container {
        position: relative;
      }

      .container > canvas {
        position: absolute;
        left: 8px;
        top: 28px;
        width: 159px;
        height: 212px;
      }

      .pixelated {
        image-rendering: crisp-edges; /* firefox */
        image-rendering: pixelated; /* chrome/webkit */
      }
    `}get canvas(){var e;return(e=this.shadowRoot)===null||e===void 0?void 0:e.querySelector("canvas")}firstUpdated(){this.dispatchEvent(new CustomEvent("canvas-ready"))}render(){const{screenWidth:e,screenHeight:i,flipHorizontal:s,flipVertical:n}=this;return G`
      <div class="container">
        <svg
          width="46.5mm"
          height="77.6mm"
          version="1.1"
          viewBox="0 0 46.5 77.6"
          xmlns="http://www.w3.org/2000/svg"
        >
          <!-- Board -->
          <path
            d="m8.8e-7 3.37e-6v77.6h46.5v-77.6zm43.1 1.78a1.8 1.8 0 0 1 1.8 1.8 1.8 1.8 0 0 1-1.8 1.8 1.8 1.8 0 0 1-1.8-1.8 1.8 1.8 0 0 1 1.8-1.8zm-39.4 0.0946a1.8 1.8 0 0 1 1.8 1.8 1.8 1.8 0 0 1-1.8 1.8 1.8 1.8 0 0 1-1.8-1.8 1.8 1.8 0 0 1 1.8-1.8zm0 70.7a1.8 1.8 0 0 1 1.8 1.8 1.8 1.8 0 0 1-1.8 1.8 1.8 1.8 0 0 1-1.8-1.8 1.8 1.8 0 0 1 1.8-1.8zm39.4 0.0946a1.8 1.8 0 0 1 1.8 1.8 1.8 1.8 0 0 1-1.8 1.8 1.8 1.8 0 0 1-1.8-1.8 1.8 1.8 0 0 1 1.8-1.8zm-31 2.68h1.41v1.34h-1.41zm2.53 0h1.41v1.34h-1.41zm2.56 0h1.41v1.34h-1.41zm2.54 0h1.41v1.34h-1.41zm12.7 0h1.41v1.34h-1.41zm-10.1 0.0119h1.41v1.34h-1.41zm2.54 0.0119h1.41v1.34h-1.41zm5.08 0h1.41v1.34h-1.41zm-2.53 0.0114h1.41v1.34h-1.41z"
            fill="#931917"
            stroke-width="0"
          />

          <!-- LCD Panel -->
          <path d="m0.17 5.65v64.6h46.1v-64.6zm6.46 62.9h34.7v1.7h-34.7z" fill="#f6e1f1" />
          <rect
            x="11.2"
            y="66.7"
            width="24.2"
            height="6.24"
            rx="1"
            ry="1"
            fill="#bdab16"
            opacity=".4"
          />
          <rect x="1.62" y="6.79" width="43.3" height="61.9" />
          <rect
            x="10.8"
            y="74.6"
            width="24.2"
            height="2.83"
            fill="none"
            stroke="#fff"
            stroke-width=".27"
          />

          <!-- Pins -->
          <g fill="#ccc">
            <path
              d="m11.8 75v1.99h1.98v-1.99zm0.988 0.397a0.6 0.6 0 0 1 0.0041 0 0.6 0.6 0 0 1 0.6 0.6 0.6 0.6 0 0 1-0.6 0.6 0.6 0.6 0 0 1-0.6-0.6 0.6 0.6 0 0 1 0.596-0.6z"
            />
            <path
              id="round-pin"
              d="m15.3 75a1 1 0 0 0-0.987 1 1 1 0 0 0 1 1 1 1 0 0 0 1-1 1 1 0 0 0-1-1 1 1 0 0 0-0.0134 0zm0.0093 0.4a0.6 0.6 0 0 1 0.0041 0 0.6 0.6 0 0 1 0.6 0.6 0.6 0.6 0 0 1-0.6 0.6 0.6 0.6 0 0 1-0.6-0.6 0.6 0.6 0 0 1 0.596-0.6z"
            />
            <use xlink:href="#round-pin" x="2.54" />
            <use xlink:href="#round-pin" x="5.08" />
            <use xlink:href="#round-pin" x="7.62" />
            <use xlink:href="#round-pin" x="10.16" />
            <use xlink:href="#round-pin" x="12.7" />
            <use xlink:href="#round-pin" x="15.24" />
            <use xlink:href="#round-pin" x="17.78" />
          </g>
          <text font-family="monospace" font-size="3.5px" fill="#fff">
            <tspan x="8.2" y="76.9">1</tspan>
            <tspan x="35.6" y="76.9">9</tspan>
            <tspan x="14.2" y="4.3" font-size="4.6px">ILI9341</tspan>
          </text>
        </svg>
        <canvas
          width="${e}"
          height="${i}"
          class="pixelated"
          style=${s||n?`transform: scaleX(${s?-1:1}) scaleY(${n?-1:1});`:""}
        ></canvas>
      </div>
    `}};ln([y()],ti.prototype,"flipHorizontal",void 0);ln([y()],ti.prototype,"flipVertical",void 0);ti=ln([U("wokwi-ili9341")],ti);var $i=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const _3=[0,1,2,3,4,5,6,7,8,9],xe=Qe,i0=1.27*xe,s0=8.83*xe,Bt="#9eff3c",C3="#2c95fa",R1="#6cf9dc",at="#f1d73c",T1="#dc012d",$3={GYR:[Bt,Bt,Bt,Bt,Bt,at,at,at,T1,T1],BCYR:[C3,R1,R1,R1,R1,at,at,at,T1,T1]};let g1=class extends L{constructor(){super(...arguments);this.color="red",this.offColor="#444",this.pinInfo=[{name:"A1",x:i0,y:1.27*xe,number:1,description:"Anode 1",signals:[]},{name:"A2",x:i0,y:3.81*xe,number:2,description:"Anode 2",signals:[]},{name:"A3",x:i0,y:6.35*xe,number:3,description:"Anode 3",signals:[]},{name:"A4",x:i0,y:8.89*xe,number:4,description:"Anode 4",signals:[]},{name:"A5",x:i0,y:11.43*xe,number:5,description:"Anode 5",signals:[]},{name:"A6",x:i0,y:13.97*xe,number:6,description:"Anode 6",signals:[]},{name:"A7",x:i0,y:16.51*xe,number:7,description:"Anode 7",signals:[]},{name:"A8",x:i0,y:19.05*xe,number:8,description:"Anode 8",signals:[]},{name:"A9",x:i0,y:21.59*xe,number:9,description:"Anode 9",signals:[]},{name:"A10",x:i0,y:24.13*xe,number:10,description:"Anode 10",signals:[]},{name:"C1",x:s0,y:1.27*xe,number:20,description:"Cathode 1",signals:[]},{name:"C2",x:s0,y:3.81*xe,number:19,description:"Cathode 2",signals:[]},{name:"C3",x:s0,y:6.35*xe,number:18,description:"Cathode 3",signals:[]},{name:"C4",x:s0,y:8.89*xe,number:17,description:"Cathode 4",signals:[]},{name:"C5",x:s0,y:11.43*xe,number:16,description:"Cathode 5",signals:[]},{name:"C6",x:s0,y:13.97*xe,number:15,description:"Cathode 6",signals:[]},{name:"C7",x:s0,y:16.51*xe,number:14,description:"Cathode 7",signals:[]},{name:"C8",x:s0,y:19.05*xe,number:13,description:"Cathode 8",signals:[]},{name:"C9",x:s0,y:21.59*xe,number:12,description:"Cathode 9",signals:[]},{name:"C10",x:s0,y:24.13*xe,number:11,description:"Cathode 10",signals:[]}],this.values=[0,0,0,0,0,0,0,0,0,0]}render(){const{values:e,color:i,offColor:s}=this,n=$3[i];return G`
      <svg
        width="10.1mm"
        height="25.5mm"
        version="1.1"
        viewBox="0 0 10.1 25.5"
        xmlns="http://www.w3.org/2000/svg"
      >
        <pattern id="pin-pattern" height="2.54" width="10.1" patternUnits="userSpaceOnUse">
          <circle cx="1.27" cy="1.27" r="0.5" fill="#aaa" />
          <circle cx="8.83" cy="1.27" r="0.5" fill="#aaa" />
        </pattern>
        <path d="m1.4 0h8.75v25.5h-10.1v-24.2z" />
        <rect width="10.1" height="25.4" fill="url(#pin-pattern)" />
        ${_3.map(r=>{var l;return V`<rect x="2.5" y="${.4+r*2.54}" width="5" height="1.74" fill="${e[r]?(l=n==null?void 0:n[r])!==null&&l!==void 0?l:i:s}"/>`})}
      </svg>
    `}};$i([y()],g1.prototype,"color",void 0);$i([y()],g1.prototype,"offColor",void 0);$i([y({type:Array})],g1.prototype,"values",void 0);g1=$i([U("wokwi-led-bar-graph")],g1);var E3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let P2=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"CD",x:76.734,y:9.3744,signals:[]},{name:"DO",x:76.734,y:18.8622,signals:[te("MISO")]},{name:"GND",x:76.734,y:28.4634,signals:[{type:"power",signal:"GND"}]},{name:"SCK",x:76.734,y:38.178,signals:[te("SCK")]},{name:"VCC",x:76.734,y:47.628,signals:[{type:"power",signal:"VCC"}]},{name:"DI",x:76.734,y:57.456,signals:[te("MOSI")]},{name:"CS",x:76.734,y:66.906,signals:[te("SS")]}]}render(){return G`
      <svg
        width="21.6mm"
        height="20.4mm"
        version="1.1"
        viewBox="0 0 21.6 20.4"
        xmlns="http://www.w3.org/2000/svg"
      >
        <!-- Board -->
        <rect width="21.6" height="20.4" fill="#a1111b" />

        <!-- Card and socket -->
        <rect
          x="1.32"
          y="4.59"
          width="3.13"
          height="8.06"
          fill="#262626"
          stroke="#d5b7b5"
          stroke-width=".232"
        />
        <rect x="4.08" y=".0367" width="10.7" height="17.8" rx=".772" ry=".772" fill="#262626" />
        <path
          d="m3.79 2.49-1.62 0.0292v1.38c0.474 0.0438 0.486 0.0027 0.489 0.197 0.0026 0.194-0.0698 0.237-1.2 0.257v0.939c0.388 0.0184 0.665-0.0702 0.679 0.211l0.176 3.66-0.413 0.434 0.0207 0.733 0.103 0.114 1.15 0.031 0.114-0.114-0.031-0.764-0.361-0.403 0.0413-3.62c0.0103-0.258 0.176-0.283 0.176-0.283h0.34c0.109 0 0.197 0.0689 0.197 0.154v5.29c0 0.0856-0.088 0.154-0.197 0.154h-1.99v6.96h5.2c-0.0402-1.41-0.191-5.31-0.0968-6 0.0155-0.114 0.263-0.0662 0.272-0.0303 0.0551 0.222 0.0216 2.55 0.234 5.65 0.431 0.234 0.17-0.655 0.423-0.635 0.242 0.0186-0.0802 0.81 0.394 0.635 0.384-4 0.0656-5.8 0.365-5.71 0.299 0.0904-0.204 5.36 0.0438 6.09h4.18s-0.17-4.16-0.285-5.96c-0.0099-0.155 0.473-0.156 0.467 0-0.0653 1.63 0.0803 5.67 0.0803 5.67 0.533 0.0657 0.168-0.755 0.46-0.745 0.279 0.0104-0.0511 0.854 0.387 0.745 0 0 0.218-4.08 0.226-5.69 7.07e-4 -0.146 0.375-0.139 0.372 0.0073-0.0227 1.42-0.131 5.97-0.131 5.97h1.01l-0.0656-14.3s-0.24-0.0383-0.352 0.0012c-3.96 1.4-7.81 1.06-10.7-0.0304-0.175-0.212-0.146-0.993-0.146-0.993z"
          fill="#dbded9"
        />
        <rect
          x="5.84"
          y="6.42"
          width="3.97"
          height="1.97"
          rx=".3"
          ry=".3"
          fill="#262626"
          stroke="#ebebeb"
          stroke-width=".08"
        />
        <rect
          x="10.4"
          y="6.42"
          width="3.97"
          height="1.97"
          rx=".3"
          ry=".3"
          fill="#262626"
          stroke="#ebebeb"
          stroke-width=".08"
        />
        <path
          d="m4.08 1.21s2.37 0.327 5.27 0.327c2.9 0 5.42-0.327 5.42-0.327v0.57s-0.949 0.331-5.42 0.327c-4.47-0.0037-5.27-0.327-5.27-0.327z"
          fill="#020202"
          stroke="#000"
          stroke-width=".027"
        />
        <g fill="#fcfff9">
          <rect x="1.19" y="10.9" width=".275" height="6.95" />
          <rect x="1.19" y="4.36" width=".275" height=".945" />
          <rect x="1.84" y="2.52" width=".33" height="1.38" />
        </g>

        <!-- Pins & labels -->
        <text font-family="sans-serif" fill="#ffffff" font-size="1.6px" stroke-width=".0327">
          <tspan x="16.61" y="2.90">CD</tspan>
          <tspan x="16.45" y="5.45">DO</tspan>
          <tspan x="15.39" y="7.98">GND</tspan>
          <tspan x="15.65" y="10.62">SCK</tspan>
          <tspan x="15.68" y="13.06">VCC</tspan>
          <tspan x="16.88" y="15.57">DI</tspan>
          <tspan x="16.67" y="18.24">CS</tspan>
        </text>
        <g fill="#fff" stroke="#d9cb97" stroke-width=".381">
          <ellipse cx="20.3" cy="2.48" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="4.99" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="7.53" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="10.1" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="12.6" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="15.2" rx=".814" ry=".814" />
          <ellipse cx="20.3" cy="17.7" rx=".814" ry=".814" />
        </g>
      </svg>
    `}};P2=E3([U("wokwi-microsd-card")],P2);var _l=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let ks=class extends L{constructor(){super(...arguments);this.values=[0,0,0,0,0,0,0,0],this.pinInfo=[{name:"1a",number:1,y:55,x:7.5,signals:[]},{name:"2a",number:2,y:55,x:17.2,signals:[]},{name:"3a",number:3,y:55,x:26.7,signals:[]},{name:"4a",number:4,y:55,x:36.5,signals:[]},{name:"5a",number:5,y:55,x:45.9,signals:[]},{name:"6a",number:6,y:55,x:55.6,signals:[]},{name:"7a",number:7,y:55,x:65.2,signals:[]},{name:"8a",number:8,y:55,x:74.8,signals:[]},{name:"8b",number:9,y:.8,x:74.8,signals:[]},{name:"7b",number:10,y:.8,x:65.2,signals:[]},{name:"6b",number:11,y:.8,x:55.6,signals:[]},{name:"5b",number:12,y:.8,x:45.9,signals:[]},{name:"4b",number:13,y:.8,x:36.5,signals:[]},{name:"3b",number:14,y:.8,x:26.7,signals:[]},{name:"2b",number:15,y:.8,x:17.2,signals:[]},{name:"1b",number:16,y:.8,x:7.5,signals:[]}]}toggleSwitch(e){this.values[e]=this.values[e]?0:1,this.dispatchEvent(new InputEvent("switch-change",{detail:e})),this.requestUpdate()}onKeyDown(e){e.stopPropagation();const s=["1","2","3","4","5","6","7","8"].indexOf(e.key);s!==-1&&this.toggleSwitch(s)}drawSwitch(e,i){return V` 
        <rect
        @click=${()=>this.toggleSwitch(e)}
        x="${i+4.693}"
        y="21.2"
        width="5.8168"
        height="13"
      />
      <use
        @click=${()=>this.toggleSwitch(e)}
        xlink:href="#switch"
        x="${i}"
        y=${this.values[e]?-7.2:0}
      />`}preventTextSelection(e){e.detail>1&&e.preventDefault()}render(){return G`
      <svg
        tabindex="0"
        @keydown=${this.onKeyDown}
        @mousedown=${this.preventTextSelection}
        width="82.87"
        height="55.355"
        version="1.1"
        viewBox="0 0 82.87 55.355"
        xmlns="http://www.w3.org/2000/svg"
      >
        <defs>
          <path
            id="switch"
            transform="translate(-66.856 -41.367)"
            fill="#fffef4"
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-width=".77094"
            d="m72.096 69.764s1.3376 0.38247 2.1066 0.39196c0.76893 0.0095 2.44-0.39196 2.44-0.39196 0.39596-0.06361 0.72389 0.32286 0.72389 0.72389v4.3678c0 0.40104-0.52337 0.72389-0.72389 0.72389s-1.6592-0.41225-2.4288-0.40316c-0.76958 0.0091-2.1177 0.40316-2.1177 0.40316-0.39396 0.075-0.72389-0.32286-0.72389-0.72389v-4.3678c0-0.40104 0.32286-0.72389 0.72389-0.72389z"
          />
        </defs>

        <!-- Pins -->
        <g
          transform="translate(-66.856 -41.367)"
          fill="#454837"
          fill-opacity=".49194"
          stroke-linecap="round"
          stroke-linejoin="round"
          stroke-width=".76744"
        >
          <rect x="73.181" y="87.634" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="83.135" y="87.455" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="92.424" y="87.639" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="102.45" y="87.639" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect
            transform="scale(1,-1)"
            x="111.55"
            y="-96.722"
            width="1.4382"
            height="9.0831"
            rx=".7206"
            ry=".7206"
          />
          <rect x="121.12" y="87.639" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="130.78" y="87.547" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="140.62" y="87.547" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="73.516" y="41.546" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="83.469" y="41.367" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="92.758" y="41.551" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="102.78" y="41.551" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect
            transform="scale(1,-1)"
            x="111.89"
            y="-50.634"
            width="1.4382"
            height="9.0831"
            rx=".7206"
            ry=".7206"
          />
          <rect x="121.45" y="41.551" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="131.11" y="41.459" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
          <rect x="140.95" y="41.459" width="1.4382" height="9.0831" rx=".7206" ry=".7206" />
        </g>

        <!-- Board -->
        <rect x="0" y="8.5" width="90.5" height="38.0831" fill="#d72c2c" />

        <!-- Text -->
        <text fill="#fffef4" font-family="sans-serif" font-size="7.66px" style="line-height:1.25">
          <tspan x="6.340" y="18.03">ON</tspan>
          <tspan x="4.35" y="43.28">1</tspan>
          <tspan x="14.485" y="43.28">2</tspan>
          <tspan x="23.956" y="43.28">3</tspan>
          <tspan x="33.57" y="43.28">4</tspan>
          <tspan x="43.05" y="43.28">5</tspan>
          <tspan x="52.36" y="43.28">6</tspan>
          <tspan x="62.45" y="43.28">7</tspan>
          <tspan x="71.92" y="43.28">8</tspan>
        </text>

        <!-- Switches -->
        <g fill="#917c6f" stroke-width=".77094">
          ${this.drawSwitch(0,0)}<!-- -->
          ${this.drawSwitch(1,9.6)}<!-- -->
          ${this.drawSwitch(2,19.4)}<!-- -->
          ${this.drawSwitch(3,29.1)}<!-- -->
          ${this.drawSwitch(4,38.5)}<!-- -->
          ${this.drawSwitch(5,48.1)}<!-- -->
          ${this.drawSwitch(6,57.7)}<!-- -->
          ${this.drawSwitch(7,67.3)}<!-- -->
        </g>
      </svg>
    `}};_l([y({type:Array})],ks.prototype,"values",void 0);ks=_l([U("wokwi-dip-switch-8")],ks);var z1=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let $t=class extends L{constructor(){super(...arguments);this.angle=0,this.arrow="",this.value="",this.units="",this.pinInfo=[{name:"A+",y:237,x:108,number:1,signals:[]},{name:"A-",y:237,x:98,number:2,signals:[]},{name:"B+",y:237,x:117,number:3,signals:[]},{name:"B-",y:237,x:127,number:4,signals:[]}]}render(){const{arrow:e}=this;return G`
      <svg
        width="57mm"
        height="63.349mm"
        version="1.1"
        viewBox="0 0 57 63.349"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <linearGradient
            id="motor-body"
            x1="17.567"
            x2="197.27"
            y1="171.8"
            y2="171.8"
            gradientTransform="matrix(.31539 0 0 .31539 9.0021 56.33)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#666" offset="0" />
            <stop stop-color="#fff" offset="1" />
          </linearGradient>
          <linearGradient
            id="linearGradient5454"
            x1="339.07"
            x2="478.3"
            y1="660.79"
            y2="660.79"
            gradientTransform="matrix(.083446 0 0 .083446 9.845 56.471)"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#9d9d9d" offset="0" />
            <stop stop-color="#9d9d9d" stop-opacity="0" offset="1" />
          </linearGradient>
          <linearGradient
            id="linearGradient29501"
            x1="33.418"
            x2="53.905"
            y1="114.39"
            y2="113.37"
            gradientUnits="userSpaceOnUse"
          >
            <stop stop-color="#9d9d9d" offset="0" />
            <stop stop-color="#fdfafa" offset=".29501" />
            <stop offset="1" />
          </linearGradient>
        </defs>

        <!-- Body -->
        <g transform="translate(-14.38 -82.486)">
          <g stroke-linecap="round" stroke-linejoin="round">
            <rect
              x="14.543"
              y="82.648"
              width="56.675"
              height="55.731"
              rx="4.7308"
              ry="4.6584"
              fill="url(#motor-body)"
              stroke="#000"
              stroke-width=".3245"
            />
            <circle cx="20.407" cy="88.675" r="1.7314" fill="#666" stroke-width=".47984" />
            <circle
              cx="65.732"
              cy="88.585"
              r="1.7314"
              fill="#666"
              stroke-linecap="round"
              stroke-linejoin="round"
              stroke-width=".47984"
            />
            <ellipse cx="65.704" cy="88.613" rx="1.0232" ry=".99314" fill="#e6e6e6" />
            <circle
              cx="66.278"
              cy="133.65"
              r="1.7314"
              fill="#666"
              stroke-linecap="round"
              stroke-linejoin="round"
              stroke-width=".47984"
            />
            <ellipse cx="66.25" cy="133.54" rx="1.0232" ry=".99314" fill="#e6e6e6" />
            <circle
              cx="19.6"
              cy="133.13"
              r="1.7314"
              fill="#666"
              stroke-linecap="round"
              stroke-linejoin="round"
              stroke-width=".47984"
            />
            <ellipse cx="19.572" cy="133.16" rx="1.0232" ry=".99314" fill="#ccc" />
            <ellipse cx="20.378" cy="88.702" rx="1.0232" ry=".99314" fill="#ccc" />

            <!-- Rotator -->
            <circle
              cx="43.816"
              cy="111.05"
              r="10.25"
              fill="#868686"
              fill-opacity=".89602"
              opacity=".73"
              stroke="url(#linearGradient29501)"
              stroke-width=".41429"
            />
            <path
              transform="rotate(${this.angle}, 43.82, 111.2)"
              fill="${e||"transparent"}"
              d="M48.706051,111.66821H38.189949L43.448,83.34119Z"
            />
            <path
              transform="rotate(${this.angle}, 43.82, 111.2)"
              d="m40.056 106.86a5.3616 5.3616 0 0 0-1.9696 4.1497 5.3616 5.3616 0 0 0 5.3616 5.3616 5.3616 5.3616 0 0 0 5.3616-5.3616 5.3616 5.3616 0 0 0-1.9672-4.1497z"
              fill="#4d4d4d"
              stroke="url(#linearGradient5454)"
              stroke-width=".57968"
            />
          </g>

          <g
            transform="matrix(-.0031398 -.26456 .26456 -.0031398 32.829 149.58)"
            clip-rule="evenodd"
            fill-rule="evenodd"
            stroke-linecap="round"
            stroke-linejoin="round"
            stroke-miterlimit="1.5"
          >
            <!-- Pins -->
            <path
              id="pin"
              fill="#9f9f9f"
              d="m15.259 56.3c-0.382-2.2e-5 -0.74801 0.15196-1.019 0.42194-0.27002 0.26998-0.42104 0.63698-0.42106 1.019-2.2e-5 0.382 0.15096 0.74801 0.42094 1.019 0.27098 0.27002 0.63698 0.42204 1.019 0.42206 5.242 2.96e-4 23.147 0.0013 26.132 0.0015 0.233 1.4e-5 0.42301-0.18998 0.42302-0.42398 3.1e-5 -0.545 8.4e-5 -1.489 1.15e-4 -2.035 1.3e-5 -0.233-0.18998-0.42301-0.42298-0.42302-2.985-1.68e-4 -20.89-0.0012-26.132-0.0015z"
            />

            <use xlink:href="#pin" y="-9.6" />
            <use xlink:href="#pin" y="-19.2" />
            <use xlink:href="#pin" y="-28.8" />

            <!-- Text -->
            <text
              transform="rotate(90)"
              font-family="arial"
              font-size="14.667px"
              text-align="center"
              text-anchor="middle"
            >
              <tspan x="45" y="-58.62619" font-size="14px">${this.units}</tspan>
              <tspan x="45" y="-75.066772" font-size="24px">${this.value}</tspan>
            </text>
          </g>
        </g>
      </svg>
    `}};z1([y()],$t.prototype,"angle",void 0);z1([y()],$t.prototype,"arrow",void 0);z1([y()],$t.prototype,"value",void 0);z1([y()],$t.prototype,"units",void 0);$t=z1([U("wokwi-stepper-motor")],$t);var Cl=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};let zs=class extends L{constructor(){super(...arguments);this.pinInfo=[{name:"VCC",y:53,x:7,number:1,signals:[De()]},{name:"DT",y:35,x:7,number:2,signals:[]},{name:"SCK",y:44,x:7,number:3,signals:[]},{name:"GND",y:25.5,x:7,number:4,signals:[$e()]}]}get dimension(){switch(this.type){case"50kg":return{width:580,height:430};case"5kg":return{width:507,height:269};case"gauge":return{width:509,height:200};default:return{width:580,height:430}}}renderSensor(){switch(this.type){case"gauge":return V`
       <!-- Gauge Pressure Sensor -->
       <g
         transform="translate(412.5 99.5) scale(-1 1) translate(-94.5 -56.5)">
         <path d="m53.5 56.5l135-56" stroke="#F01919" stroke-linecap="square" stroke-width="3" />
         <path d="m54.5 56.5l133-33" stroke="#000" stroke-linecap="square" stroke-width="3" />
         <path d="m52.5 56.5l133-7" stroke="#C8C8C8" stroke-linecap="square" stroke-width="3" />
         <path d="m51.5 56.5l136 18" stroke="#24C22B" stroke-linecap="square" stroke-width="3" />
         <path
           d="m50.5 113c-14.636 0-26.5-25.296-26.5-56.5s11.864-56.5 26.5-56.5 26.5 25.296 26.5 56.5-11.864 56.5-26.5 56.5zm-1-88c3.5899 0 6.5-3.134 6.5-7s-2.9101-7-6.5-7-6.5 3.134-6.5 7 2.9101 7 6.5 7zm0 77c3.5899 0 6.5-3.134 6.5-7s-2.9101-7-6.5-7-6.5 3.134-6.5 7 2.9101 7 6.5 7z"
           fill="#D8D8D8"
           stroke="#979797"
         />
         <path d="m0 54.1l33-5.1v17l-33-5.1v-6.8z" fill="#D8D8D8" stroke="#979797" />
         <circle cx="50" cy="57" r="20" fill="#D8D8D8" stroke="#979797" />
         <text
           transform="translate(50 56.5) scale(-1 1) translate(-50 -56.5)"
           fill="#FBFBFB"
           font-family="Arial-BoldItalicMT, Arial"
           font-size="16"
           font-style="italic"
           font-weight="bold"
         >
           <tspan x="38.4414062" y="62">GP</tspan>
         </text>
       </g>`;case"5kg":return V`
       <!-- 5kg Load Sensor -->
       <g
         transform="translate(413 133.5) scale(-1 1) translate(-94 -133.5)">
         <path
           d="m0.5 0.5v266h62v-266h-62zm31 123c-12.698 0-23-9.8445-23-22 0-12.155 10.302-22 23-22s23 9.8445 23 22c0 12.155-10.302 22-23 22zm0 57c-12.698 0-23-9.8445-23-22s10.302-22 23-22 23 9.8445 23 22-10.302 22-23 22z"
           fill="#D8D8D8"
           stroke="#979797"
         />
         <path d="m53.5 51.5l133-10" stroke="#F01919" stroke-linecap="square" stroke-width="3" />
         <path d="m54.5 57.5l131 8" stroke="#000" stroke-linecap="square" stroke-width="3" />
         <path d="m55.5 64.5l130 27" stroke="#C8C8C8" stroke-linecap="square" stroke-width="3" />
         <path d="m56.5 71.5l131 43" stroke="#24C22B" stroke-linecap="square" stroke-width="3" />
         <rect x="3.5" y="43.5" width="57" height="33" rx="11" fill="url(#e)" stroke="#979797" />
         <text
           transform="translate(28.5 245) scale(-1 1) translate(-28.5 -245)"
           fill="#FBFBFB"
           font-family="Arial-ItalicMT, Arial"
           font-size="14"
           font-style="italic"
         >
           <tspan x="17.2138672" y="250">5kg</tspan>
         </text>
       </g>`;case"50kg":default:return V`
       <!-- 50kg Load Sensor -->
       <g
         transform="translate(448 212) scale(-1 1) translate(-131 -212)"
       >
         <path
           d="m30 0.5c-16.292 0-29.5 13.208-29.5 29.5v46c0 16.292 13.208 29.5 29.5 29.5h46c16.292 0 29.5-13.208 29.5-29.5v-46c0-16.292-13.208-29.5-29.5-29.5h-46zm-3 9h40c9.1127 0 16.5 7.3873 16.5 16.5v54c0 9.1127-7.3873 16.5-16.5 16.5h-40c-9.1127 0-16.5-7.3873-16.5-16.5v-54c0-9.1127 7.3873-16.5 16.5-16.5z"
           fill="#D8D8D8"
           stroke="#979797"
         />
         <g transform="translate(18 17)" fill="#D8D8D8">
           <path
             d="m63 48h-36c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36c0 7.1797-5.8203 13-13 13h-37c-7.1797 0-13-5.8203-13-13v-43c-8.7926e-16 -7.1797 5.8203-13 13-13h37c6.8432 0 12.451 5.2876 12.962 12h0.03789v1c0-0.33647-0.012783-0.66996-0.03789-1h-35.962c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36v24z"
           />
           <rect x="61" y="24" width="13" height="24" />
           <rect x="30.5" y=".5" width="32" height="72" rx="11" stroke="#979797" />
         </g>
         <circle cx="64.5" cy="53.5" r="7.5" fill="url(#b)" />
         <ellipse cx="64.5" cy="25" rx="4.5" ry="4" fill="#737373" />
         <ellipse cx="64.5" cy="82" rx="4.5" ry="4" fill="#737373" />
         <path d="m106.5 48.25h21" stroke="#979797" stroke-linecap="square" stroke-width="3" />
         <path d="m106.5 53.25h21" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m106.5 58.25h21" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         <g transform="translate(0 106)">
           <path
             d="m30 0.5c-16.292 0-29.5 13.208-29.5 29.5v46c0 16.292 13.208 29.5 29.5 29.5h46c16.292 0 29.5-13.208 29.5-29.5v-46c0-16.292-13.208-29.5-29.5-29.5h-46zm-3 9h40c9.1127 0 16.5 7.3873 16.5 16.5v54c0 9.1127-7.3873 16.5-16.5 16.5h-40c-9.1127 0-16.5-7.3873-16.5-16.5v-54c0-9.1127 7.3873-16.5 16.5-16.5z"
             fill="#D8D8D8"
             stroke="#979797"
           />
           <g transform="translate(18 17)" fill="#D8D8D8">
             <path
               d="m63 48h-36c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36c0 7.1797-5.8203 13-13 13h-37c-7.1797 0-13-5.8203-13-13v-43c-8.7926e-16 -7.1797 5.8203-13 13-13h37c6.8432 0 12.451 5.2876 12.962 12h0.03789v1c0-0.33647-0.012783-0.66996-0.03789-1h-35.962c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36v24z"
             />
             <rect x="61" y="24" width="13" height="24" />
             <rect x="30.5" y=".5" width="32" height="72" rx="11" stroke="#979797" />
           </g>
           <circle cx="64.5" cy="53.5" r="7.5" fill="url(#b)" />
           <ellipse cx="64.5" cy="25" rx="4.5" ry="4" fill="#737373" />
           <ellipse cx="64.5" cy="82" rx="4.5" ry="4" fill="#737373" />
           <path d="m106.5 48.25h21" stroke="#979797" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 53.25h21" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 58.25h21" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         </g>
         <path
           d="m30 0.5c-16.292 0-29.5 13.208-29.5 29.5v46c0 16.292 13.208 29.5 29.5 29.5h46c16.292 0 29.5-13.208 29.5-29.5v-46c0-16.292-13.208-29.5-29.5-29.5h-46zm-3 9h40c9.1127 0 16.5 7.3873 16.5 16.5v54c0 9.1127-7.3873 16.5-16.5 16.5h-40c-9.1127 0-16.5-7.3873-16.5-16.5v-54c0-9.1127 7.3873-16.5 16.5-16.5z"
           fill="#D8D8D8"
           stroke="#979797"
         />
         <g transform="translate(18 17)" fill="#D8D8D8">
           <path
             d="m63 48h-36c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36c0 7.1797-5.8203 13-13 13h-37c-7.1797 0-13-5.8203-13-13v-43c-8.7926e-16 -7.1797 5.8203-13 13-13h37c6.8432 0 12.451 5.2876 12.962 12h0.03789v1c0-0.33647-0.012783-0.66996-0.03789-1h-35.962c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36v24z"
           />
           <rect x="61" y="24" width="13" height="24" />
           <rect x="30.5" y=".5" width="32" height="72" rx="11" stroke="#979797" />
         </g>
         <circle cx="64.5" cy="53.5" r="7.5" fill="url(#b)" />
         <ellipse cx="64.5" cy="25" rx="4.5" ry="4" fill="#737373" />
         <ellipse cx="64.5" cy="82" rx="4.5" ry="4" fill="#737373" />
         <path d="m106.5 48.25h21" stroke="#979797" stroke-linecap="square" stroke-width="3" />
         <path d="m106.5 53.25h21" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m106.5 58.25h21" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         <g transform="translate(0 212)">
           <path
             d="m30 0.5c-16.292 0-29.5 13.208-29.5 29.5v46c0 16.292 13.208 29.5 29.5 29.5h46c16.292 0 29.5-13.208 29.5-29.5v-46c0-16.292-13.208-29.5-29.5-29.5h-46zm-3 9h40c9.1127 0 16.5 7.3873 16.5 16.5v54c0 9.1127-7.3873 16.5-16.5 16.5h-40c-9.1127 0-16.5-7.3873-16.5-16.5v-54c0-9.1127 7.3873-16.5 16.5-16.5z"
             fill="#D8D8D8"
             stroke="#979797"
           />
           <g transform="translate(18 17)" fill="#D8D8D8">
             <path
               d="m63 48h-36c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36c0 7.1797-5.8203 13-13 13h-37c-7.1797 0-13-5.8203-13-13v-43c-8.7926e-16 -7.1797 5.8203-13 13-13h37c6.8432 0 12.451 5.2876 12.962 12h0.03789v1c0-0.33647-0.012783-0.66996-0.03789-1h-35.962c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36v24z"
             />
             <rect x="61" y="24" width="13" height="24" />
             <rect x="30.5" y=".5" width="32" height="72" rx="11" stroke="#979797" />
           </g>
           <circle cx="64.5" cy="53.5" r="7.5" fill="url(#b)" />
           <ellipse cx="64.5" cy="25" rx="4.5" ry="4" fill="#737373" />
           <ellipse cx="64.5" cy="82" rx="4.5" ry="4" fill="#737373" />
           <path d="m106.5 48.25h21" stroke="#979797" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 53.25h21" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 58.25h21" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         </g>
         <g transform="translate(0 318)">
           <path
             d="m30 0.5c-16.292 0-29.5 13.208-29.5 29.5v46c0 16.292 13.208 29.5 29.5 29.5h46c16.292 0 29.5-13.208 29.5-29.5v-46c0-16.292-13.208-29.5-29.5-29.5h-46zm-3 9h40c9.1127 0 16.5 7.3873 16.5 16.5v54c0 9.1127-7.3873 16.5-16.5 16.5h-40c-9.1127 0-16.5-7.3873-16.5-16.5v-54c0-9.1127 7.3873-16.5 16.5-16.5z"
             fill="#D8D8D8"
             stroke="#979797"
           />
           <g transform="translate(18 17)" fill="#D8D8D8">
             <path
               d="m63 48h-36c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36c0 7.1797-5.8203 13-13 13h-37c-7.1797 0-13-5.8203-13-13v-43c-8.7926e-16 -7.1797 5.8203-13 13-13h37c6.8432 0 12.451 5.2876 12.962 12h0.03789v1c0-0.33647-0.012783-0.66996-0.03789-1h-35.962c-2.7614 0-5 2.2386-5 5s2.2386 5 5 5h36v24z"
             />
             <rect x="61" y="24" width="13" height="24" />
             <rect x="30.5" y=".5" width="32" height="72" rx="11" stroke="#979797" />
           </g>
           <circle cx="64.5" cy="53.5" r="7.5" fill="url(#b)" />
           <ellipse cx="64.5" cy="25" rx="4.5" ry="4" fill="#737373" />
           <ellipse cx="64.5" cy="82" rx="4.5" ry="4" fill="#737373" />
           <path d="m106.5 48.25h21" stroke="#979797" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 53.25h21" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
           <path d="m106.5 58.25h21" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         </g>
         <path d="m128.5 53.5h82" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m201.5 68.5h60" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m211.5 44.5h50" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m213.5 94.5h48" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m225.5 118.5h36" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m128.5 371.25h94" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m225.5 118.5v253" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m128.5 265.25h83" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m213.5 96.5v169" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m201.5 71.5v88" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m211.5 44.5v9" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m128.5 159.25h71" stroke="#FF7F7F" stroke-linecap="square" stroke-width="3" />
         <path d="m136.5 164.5v212" stroke="#D3D1D1" stroke-linecap="square" stroke-width="3" />
         <path d="m147.25 58v212" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         <path d="m162.25 48v317.5" stroke="#8A8198" stroke-linecap="square" stroke-width="3" />
         <path d="m180.25 154.5v104" stroke="#999B7D" stroke-linecap="square" stroke-width="3" />
         <path d="m129 376.23h6" stroke="#D3D1D1" stroke-linecap="square" stroke-width="3" />
         <path d="m136.5 164.15h-8" stroke="#D3D1D1" stroke-linecap="square" stroke-width="3" />
         <path d="m146.5 58.15h-18" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         <path d="m161.5 48.208h-34" stroke="#8A8198" stroke-linecap="square" stroke-width="3" />
         <path d="m180.5 154.1h-52" stroke="#999B7D" stroke-linecap="square" stroke-width="3" />
         <path d="m180.5 260.2h-51" stroke="#999B7D" stroke-linecap="square" stroke-width="3" />
         <path d="m161.5 366.2h-34" stroke="#8A8198" stroke-linecap="square" stroke-width="3" />
         <path d="m146.5 270.25h-18" stroke="#EFEFEF" stroke-linecap="square" stroke-width="3" />
         <text
           transform="translate(94.5 265) scale(-1 1) translate(-94.5 -265)"
           fill="#FBFBFB"
           font-family="Arial-BoldItalicMT, Arial"
           font-size="14"
           font-style="italic"
           font-weight="bold"
         >
           <tspan x="87.1137695" y="270">A-</tspan>
           <tspan x="87.5" y="164">E-</tspan>
           <tspan x="85.7431641" y="57">E+</tspan>
           <tspan x="86.2431641" y="377">A+</tspan>
         </text>
       </g>`}}render(){const{width:e,height:i}=this.dimension;return G`
      <style>
        text {
          user-select: none;
        }
      </style>
      <svg
        width="${+e/10}mm"
        height="${+i/10}mm"
        viewBox="0 0 ${e} ${i}"
        version="1.1"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink"
      >
        <defs>
          <path
            id="f"
            d="m317 74c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0 25c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0 24c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0 23c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0 24c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0-120c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm-298 98c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0-24c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0-25c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm0-24c-3.866 0-7-3.134-7-7s3.134-7 7-7 7 3.134 7 7-3.134 7-7 7zm9.5 115c-6.3513 0-11.5-5.1487-11.5-11.5s5.1487-11.5 11.5-11.5 11.5 5.1487 11.5 11.5-5.1487 11.5-11.5 11.5zm0-156c-6.3513 0-11.5-5.1487-11.5-11.5s5.1487-11.5 11.5-11.5 11.5 5.1487 11.5 11.5-5.1487 11.5-11.5 11.5zm-28.5-34h340v200h-340v-200z"
          />
          <path id="d" d="m0 0h340v200h-340v-200z" />
          <mask id="g" x="0" y="0" width="340" height="200" fill="white">
            <use xlink:href="#d" />
          </mask>
          <linearGradient id="a" y1="50%" y2="50%">
            <stop stop-color="#767676" offset="0" />
            <stop stop-color="#FFFEDF" offset="1" />
          </linearGradient>
          <radialGradient id="b" r="100%">
            <stop stop-color="#8A8A8A" offset="0" />
            <stop offset="1" />
          </radialGradient>
          <radialGradient
            id="e"
            r="100%"
            gradientTransform="translate(.5 .5) scale(.58621 1) rotate(90) translate(-.5 -.5)"
          >
            <stop stop-color="#fff" offset="0" />
            <stop stop-color="#E7E7E7" offset="1" />
          </radialGradient>
        </defs>
        <g fill="none" fill-rule="evenodd">
          <mask id="c" fill="white">
            <use xlink:href="#f" />
          </mask>
          <g
            fill="#1C8944"
            mask="url(#c)"
            stroke="#ADA216"
            stroke-dasharray="1,1"
            stroke-opacity=".53643"
            stroke-width="4"
          >
            <use mask="url(#g)" xlink:href="#d" />
          </g>
          <g fill="#D2CDC3" mask="url(#c)">
            <g transform="translate(10 34)">
              <rect y="25" width="17" height="18" />
              <rect x="298" width="17" height="18" />
            </g>
          </g>
          <g mask="url(#c)">
            <g transform="translate(6 27)">
              <rect x="298" y="1" width="26" height="147" stroke="#fff" stroke-width="2" />
              <rect x="1" y="25" width="25" height="102" stroke="#fff" stroke-width="2" />
              <text
                font-family="Arial-BoldItalicMT, Arial"
                font-size="14"
                font-style="italic"
                font-weight="bold"
              >
                <tspan x="28.4448242" y="45" fill="#FBFBFB">GND</tspan>
                <tspan x="28.1689453" y="69" fill="#FBFBFB">DT</tspan>
                <tspan x="27.2207031" y="94" fill="#FBFBFB">SCK</tspan>
                <tspan x="28.2207031" y="119" fill="#FBFBFB">VCC</tspan>
                <tspan x="274" y="45" fill="#FBFBFB">E-</tspan>
                <tspan x="274.492188" y="18" fill="#FBFBFB">E+</tspan>
                <tspan x="274.058594" y="117" fill="#FBFBFB">B-</tspan>
                <tspan x="274.050781" y="140" fill="#FBFBFB">B+</tspan>
                <tspan x="274.058594" y="69" fill="#FBFBFB">A-</tspan>
                <tspan x="274.050781" y="94" fill="#FBFBFB">A+</tspan>
                <tspan x="98.2929688" y="160" fill="#FBFBFB">Load Cell Amp</tspan>
                <tspan x="126.132812" y="140" fill="#FBFBFB">HX711</tspan>
              </text>
            </g>
          </g>
          <g mask="url(#c)">
            <g transform="translate(122 28)">
              <rect
                x="14.5"
                y=".5"
                width="36"
                height="98"
                fill="#434341"
                fill-rule="evenodd"
                stroke="#3B3939"
              />
              <rect x="51" y="2" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="15" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="27" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="40" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="52" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="65" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="77" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect x="51" y="90" width="14" height="6" fill="url(#a)" fill-rule="evenodd" />
              <rect
                transform="translate(7 5) scale(-1 1) translate(-7 -5)"
                y="2"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 18) scale(-1 1) translate(-7 -18)"
                y="15"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 31) scale(-1 1) translate(-7 -31)"
                y="28"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 43) scale(-1 1) translate(-7 -43)"
                y="40"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 56) scale(-1 1) translate(-7 -56)"
                y="53"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 68) scale(-1 1) translate(-7 -68)"
                y="65"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 80) scale(-1 1) translate(-7 -80)"
                y="77"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <rect
                transform="translate(7 93) scale(-1 1) translate(-7 -93)"
                y="90"
                width="14"
                height="6"
                fill="url(#a)"
                fill-rule="evenodd"
              />
              <circle
                cx="20"
                cy="5"
                r="3"
                fill="#211919"
                fill-opacity=".54978"
                fill-rule="evenodd"
                stroke="#000"
              />
              <text
                transform="translate(27 52) rotate(-90) translate(-27 -52)"
                fill="none"
                font-family="Arial-BoldItalicMT, Arial"
                font-size="12"
                font-style="italic"
                font-weight="bold"
              >
                <tspan x="9.09960938" y="56" fill="#BBBBBB">HX711</tspan>
              </text>
            </g>
          </g>
          ${this.renderSensor()}
        </g>
      </svg>
    `}};Cl([y()],zs.prototype,"type",void 0);zs=Cl([U("wokwi-hx711")],zs);var S3=globalThis&&globalThis.__decorate||function(t,e,i,s){var n=arguments.length,r=n<3?e:s===null?s=Object.getOwnPropertyDescriptor(e,i):s,l;if(typeof Reflect=="object"&&typeof Reflect.decorate=="function")r=Reflect.decorate(t,e,i,s);else for(var o=t.length-1;o>=0;o--)(l=t[o])&&(r=(n<3?l(r):n>3?l(e,i,r):l(e,i))||r);return n>3&&r&&Object.defineProperty(e,i,r),r};const M1=5.1,F1=32.7,D2=7.6,A2=25.75,O2=44,R2=71.8;let T2=class extends L{get pinInfo(){return[{name:"NO2",x:D2,y:M1,signals:[],number:8},{name:"NC2",x:A2,y:M1,signals:[],number:6},{name:"P2",x:O2,y:M1,signals:[],number:4},{name:"COIL2",x:R2,y:M1,signals:[{type:"power",signal:"GND"}],number:1},{name:"NO1",x:D2,y:F1,signals:[],number:9},{name:"NC1",x:A2,y:F1,signals:[],number:11},{name:"P1",x:O2,y:F1,signals:[],number:13},{name:"COIL1",x:R2,y:F1,signals:[],number:16}]}render(){return G`
      <svg
        width="21mm"
        height="10mm"
        version="1.1"
        viewBox="0 0 21 10"
        xmlns="http://www.w3.org/2000/svg"
      >
        <g stroke-width=".4" fill="#f7b93c" stroke="#dda137">
          <rect x=".20" y=".20" width="20.6" height="9.61" ry=".58" />
          <rect x="20.2" y="4.5" width=".40" height="1" fill="#dda137" />
        </g>
        <g fill="none" stroke="#dda137" stroke-width=".47">
          <ellipse cx="2" cy="1.35" rx=".76" ry=".76" />
          <ellipse cx="6.8244" cy="1.35" rx=".76" ry=".76" />
          <ellipse cx="11.649" cy="1.35" rx=".76" ry=".76" />
          <ellipse cx="19" cy="1.35" rx=".76" ry=".76" />
          <ellipse cx="2" cy="8.65" rx=".76" ry=".76" />
          <ellipse cx="6.8244" cy="8.65" rx=".76" ry=".76" />
          <ellipse cx="11.649" cy="8.65" rx=".76" ry=".76" />
          <ellipse cx="19" cy="8.65" rx=".76" ry=".76" />
        </g>
        <text fill="#000" font-family="sans-serif" font-size="2.8222px">
          <tspan x="1.07" y="6.03">KS2E-M-DC5</tspan>
        </text>
      </svg>
    `}};T2=S3([U("wokwi-ks2e-m-dc5")],T2);var P3=(t,e)=>{const i=t.__vccOpts||t;for(const[s,n]of e)i[s]=n;return i};const D3=Us({name:"App"});function A3(t,e,i,s,n,r){const l=ro("router-view");return Xr(),Yr(l)}var O3=P3(D3,[["render",A3]]);var R3=!1;/*!
  * pinia v2.0.14
  * (c) 2022 Eduardo San Martin Morote
  * @license MIT
  */const T3=t=>t,M3=Symbol();var M2;(function(t){t.direct="direct",t.patchObject="patch object",t.patchFunction="patch function"})(M2||(M2={}));function F3(){const t=t4(!0),e=t.run(()=>Ls({}));let i=[],s=[];const n=ai({install(r){T3(n),n._a=r,r.provide(M3,n),r.config.globalProperties.$pinia=n,s.forEach(l=>i.push(l)),s=[]},use(r){return!this._a&&!R3?s.push(r):i.push(r),this},_p:i,_a:null,_e:t,_s:new Map,state:e});return n}var qi=()=>F3();/*!
  * vue-router v4.0.15
  * (c) 2022 Eduardo San Martin Morote
  * @license MIT
  */const $l=typeof Symbol=="function"&&typeof Symbol.toStringTag=="symbol",Tt=t=>$l?Symbol(t):"_vr_"+t,I3=Tt("rvlm"),F2=Tt("rvd"),on=Tt("r"),El=Tt("rl"),_s=Tt("rvl"),pt=typeof window!="undefined";function j3(t){return t.__esModule||$l&&t[Symbol.toStringTag]==="Module"}const fe=Object.assign;function Ki(t,e){const i={};for(const s in e){const n=e[s];i[s]=Array.isArray(n)?n.map(t):t(n)}return i}const Jt=()=>{},B3=/\/$/,N3=t=>t.replace(B3,"");function Xi(t,e,i="/"){let s,n={},r="",l="";const o=e.indexOf("?"),a=e.indexOf("#",o>-1?o:0);return o>-1&&(s=e.slice(0,o),r=e.slice(o+1,a>-1?a:e.length),n=t(r)),a>-1&&(s=s||e.slice(0,a),l=e.slice(a,e.length)),s=V3(s!=null?s:e,i),{fullPath:s+(r&&"?")+r+l,path:s,query:n,hash:l}}function L3(t,e){const i=e.query?t(e.query):"";return e.path+(i&&"?")+i+(e.hash||"")}function I2(t,e){return!e||!t.toLowerCase().startsWith(e.toLowerCase())?t:t.slice(e.length)||"/"}function H3(t,e,i){const s=e.matched.length-1,n=i.matched.length-1;return s>-1&&s===n&&Et(e.matched[s],i.matched[n])&&Sl(e.params,i.params)&&t(e.query)===t(i.query)&&e.hash===i.hash}function Et(t,e){return(t.aliasOf||t)===(e.aliasOf||e)}function Sl(t,e){if(Object.keys(t).length!==Object.keys(e).length)return!1;for(const i in t)if(!U3(t[i],e[i]))return!1;return!0}function U3(t,e){return Array.isArray(t)?j2(t,e):Array.isArray(e)?j2(e,t):t===e}function j2(t,e){return Array.isArray(e)?t.length===e.length&&t.every((i,s)=>i===e[s]):t.length===1&&t[0]===e}function V3(t,e){if(t.startsWith("/"))return t;if(!t)return e;const i=e.split("/"),s=t.split("/");let n=i.length-1,r,l;for(r=0;r<s.length;r++)if(l=s[r],!(n===1||l==="."))if(l==="..")n--;else break;return i.slice(0,n).join("/")+"/"+s.slice(r-(r===s.length?1:0)).join("/")}var y1;(function(t){t.pop="pop",t.push="push"})(y1||(y1={}));var Qt;(function(t){t.back="back",t.forward="forward",t.unknown=""})(Qt||(Qt={}));function G3(t){if(!t)if(pt){const e=document.querySelector("base");t=e&&e.getAttribute("href")||"/",t=t.replace(/^\w+:\/\/[^\/]+/,"")}else t="/";return t[0]!=="/"&&t[0]!=="#"&&(t="/"+t),N3(t)}const q3=/^[^#]+#/;function K3(t,e){return t.replace(q3,"#")+e}function X3(t,e){const i=document.documentElement.getBoundingClientRect(),s=t.getBoundingClientRect();return{behavior:e.behavior,left:s.left-i.left-(e.left||0),top:s.top-i.top-(e.top||0)}}const Ei=()=>({left:window.pageXOffset,top:window.pageYOffset});function W3(t){let e;if("el"in t){const i=t.el,s=typeof i=="string"&&i.startsWith("#"),n=typeof i=="string"?s?document.getElementById(i.slice(1)):document.querySelector(i):i;if(!n)return;e=X3(n,t)}else e=t;"scrollBehavior"in document.documentElement.style?window.scrollTo(e):window.scrollTo(e.left!=null?e.left:window.pageXOffset,e.top!=null?e.top:window.pageYOffset)}function B2(t,e){return(history.state?history.state.position-e:-1)+t}const Cs=new Map;function Y3(t,e){Cs.set(t,e)}function Z3(t){const e=Cs.get(t);return Cs.delete(t),e}let J3=()=>location.protocol+"//"+location.host;function Pl(t,e){const{pathname:i,search:s,hash:n}=e,r=t.indexOf("#");if(r>-1){let o=n.includes(t.slice(r))?t.slice(r).length:1,a=n.slice(o);return a[0]!=="/"&&(a="/"+a),I2(a,"")}return I2(i,t)+s+n}function Q3(t,e,i,s){let n=[],r=[],l=null;const o=({state:d})=>{const g=Pl(t,location),x=i.value,A=e.value;let C=0;if(d){if(i.value=g,e.value=d,l&&l===x){l=null;return}C=A?d.position-A.position:0}else s(g);n.forEach(D=>{D(i.value,x,{delta:C,type:y1.pop,direction:C?C>0?Qt.forward:Qt.back:Qt.unknown})})};function a(){l=i.value}function c(d){n.push(d);const g=()=>{const x=n.indexOf(d);x>-1&&n.splice(x,1)};return r.push(g),g}function h(){const{history:d}=window;!d.state||d.replaceState(fe({},d.state,{scroll:Ei()}),"")}function p(){for(const d of r)d();r=[],window.removeEventListener("popstate",o),window.removeEventListener("beforeunload",h)}return window.addEventListener("popstate",o),window.addEventListener("beforeunload",h),{pauseListeners:a,listen:c,destroy:p}}function N2(t,e,i,s=!1,n=!1){return{back:t,current:e,forward:i,replaced:s,position:window.history.length,scroll:n?Ei():null}}function ec(t){const{history:e,location:i}=window,s={value:Pl(t,i)},n={value:e.state};n.value||r(s.value,{back:null,current:s.value,forward:null,position:e.length-1,replaced:!0,scroll:null},!0);function r(a,c,h){const p=t.indexOf("#"),d=p>-1?(i.host&&document.querySelector("base")?t:t.slice(p))+a:J3()+t+a;try{e[h?"replaceState":"pushState"](c,"",d),n.value=c}catch(g){console.error(g),i[h?"replace":"assign"](d)}}function l(a,c){const h=fe({},e.state,N2(n.value.back,a,n.value.forward,!0),c,{position:n.value.position});r(a,h,!0),s.value=a}function o(a,c){const h=fe({},n.value,e.state,{forward:a,scroll:Ei()});r(h.current,h,!0);const p=fe({},N2(s.value,a,null),{position:h.position+1},c);r(a,p,!1),s.value=a}return{location:s,state:n,push:o,replace:l}}function tc(t){t=G3(t);const e=ec(t),i=Q3(t,e.state,e.location,e.replace);function s(r,l=!0){l||i.pauseListeners(),history.go(r)}const n=fe({location:"",base:t,go:s,createHref:K3.bind(null,t)},e,i);return Object.defineProperty(n,"location",{enumerable:!0,get:()=>e.location.value}),Object.defineProperty(n,"state",{enumerable:!0,get:()=>e.state.value}),n}function ic(t){return typeof t=="string"||t&&typeof t=="object"}function Dl(t){return typeof t=="string"||typeof t=="symbol"}const w0={path:"/",name:void 0,params:{},query:{},hash:"",fullPath:"/",matched:[],meta:{},redirectedFrom:void 0},Al=Tt("nf");var L2;(function(t){t[t.aborted=4]="aborted",t[t.cancelled=8]="cancelled",t[t.duplicated=16]="duplicated"})(L2||(L2={}));function St(t,e){return fe(new Error,{type:t,[Al]:!0},e)}function b0(t,e){return t instanceof Error&&Al in t&&(e==null||!!(t.type&e))}const H2="[^/]+?",sc={sensitive:!1,strict:!1,start:!0,end:!0},nc=/[.+*?^${}()[\]/\\]/g;function rc(t,e){const i=fe({},sc,e),s=[];let n=i.start?"^":"";const r=[];for(const c of t){const h=c.length?[]:[90];i.strict&&!c.length&&(n+="/");for(let p=0;p<c.length;p++){const d=c[p];let g=40+(i.sensitive?.25:0);if(d.type===0)p||(n+="/"),n+=d.value.replace(nc,"\\$&"),g+=40;else if(d.type===1){const{value:x,repeatable:A,optional:C,regexp:D}=d;r.push({name:x,repeatable:A,optional:C});const z=D||H2;if(z!==H2){g+=10;try{new RegExp(`(${z})`)}catch(P){throw new Error(`Invalid custom RegExp for param "${x}" (${z}): `+P.message)}}let O=A?`((?:${z})(?:/(?:${z}))*)`:`(${z})`;p||(O=C&&c.length<2?`(?:/${O})`:"/"+O),C&&(O+="?"),n+=O,g+=20,C&&(g+=-8),A&&(g+=-20),z===".*"&&(g+=-50)}h.push(g)}s.push(h)}if(i.strict&&i.end){const c=s.length-1;s[c][s[c].length-1]+=.7000000000000001}i.strict||(n+="/?"),i.end?n+="$":i.strict&&(n+="(?:/|$)");const l=new RegExp(n,i.sensitive?"":"i");function o(c){const h=c.match(l),p={};if(!h)return null;for(let d=1;d<h.length;d++){const g=h[d]||"",x=r[d-1];p[x.name]=g&&x.repeatable?g.split("/"):g}return p}function a(c){let h="",p=!1;for(const d of t){(!p||!h.endsWith("/"))&&(h+="/"),p=!1;for(const g of d)if(g.type===0)h+=g.value;else if(g.type===1){const{value:x,repeatable:A,optional:C}=g,D=x in c?c[x]:"";if(Array.isArray(D)&&!A)throw new Error(`Provided param "${x}" is an array but it is not repeatable (* or + modifiers)`);const z=Array.isArray(D)?D.join("/"):D;if(!z)if(C)d.length<2&&t.length>1&&(h.endsWith("/")?h=h.slice(0,-1):p=!0);else throw new Error(`Missing required param "${x}"`);h+=z}}return h}return{re:l,score:s,keys:r,parse:o,stringify:a}}function lc(t,e){let i=0;for(;i<t.length&&i<e.length;){const s=e[i]-t[i];if(s)return s;i++}return t.length<e.length?t.length===1&&t[0]===40+40?-1:1:t.length>e.length?e.length===1&&e[0]===40+40?1:-1:0}function oc(t,e){let i=0;const s=t.score,n=e.score;for(;i<s.length&&i<n.length;){const r=lc(s[i],n[i]);if(r)return r;i++}return n.length-s.length}const ac={type:0,value:""},cc=/[a-zA-Z0-9_]/;function hc(t){if(!t)return[[]];if(t==="/")return[[ac]];if(!t.startsWith("/"))throw new Error(`Invalid path "${t}"`);function e(g){throw new Error(`ERR (${i})/"${c}": ${g}`)}let i=0,s=i;const n=[];let r;function l(){r&&n.push(r),r=[]}let o=0,a,c="",h="";function p(){!c||(i===0?r.push({type:0,value:c}):i===1||i===2||i===3?(r.length>1&&(a==="*"||a==="+")&&e(`A repeatable param (${c}) must be alone in its segment. eg: '/:ids+.`),r.push({type:1,value:c,regexp:h,repeatable:a==="*"||a==="+",optional:a==="*"||a==="?"})):e("Invalid state to consume buffer"),c="")}function d(){c+=a}for(;o<t.length;){if(a=t[o++],a==="\\"&&i!==2){s=i,i=4;continue}switch(i){case 0:a==="/"?(c&&p(),l()):a===":"?(p(),i=1):d();break;case 4:d(),i=s;break;case 1:a==="("?i=2:cc.test(a)?d():(p(),i=0,a!=="*"&&a!=="?"&&a!=="+"&&o--);break;case 2:a===")"?h[h.length-1]=="\\"?h=h.slice(0,-1)+a:i=3:h+=a;break;case 3:p(),i=0,a!=="*"&&a!=="?"&&a!=="+"&&o--,h="";break;default:e("Unknown state");break}}return i===2&&e(`Unfinished custom RegExp for param "${c}"`),p(),l(),n}function fc(t,e,i){const s=rc(hc(t.path),i),n=fe(s,{record:t,parent:e,children:[],alias:[]});return e&&!n.record.aliasOf==!e.record.aliasOf&&e.children.push(n),n}function dc(t,e){const i=[],s=new Map;e=V2({strict:!1,end:!0,sensitive:!1},e);function n(h){return s.get(h)}function r(h,p,d){const g=!d,x=uc(h);x.aliasOf=d&&d.record;const A=V2(e,h),C=[x];if("alias"in h){const O=typeof h.alias=="string"?[h.alias]:h.alias;for(const P of O)C.push(fe({},x,{components:d?d.record.components:x.components,path:P,aliasOf:d?d.record:x}))}let D,z;for(const O of C){const{path:P}=O;if(p&&P[0]!=="/"){const W=p.record.path,J=W[W.length-1]==="/"?"":"/";O.path=p.record.path+(P&&J+P)}if(D=fc(O,p,A),d?d.alias.push(D):(z=z||D,z!==D&&z.alias.push(D),g&&h.name&&!U2(D)&&l(h.name)),"children"in x){const W=x.children;for(let J=0;J<W.length;J++)r(W[J],D,d&&d.children[J])}d=d||D,a(D)}return z?()=>{l(z)}:Jt}function l(h){if(Dl(h)){const p=s.get(h);p&&(s.delete(h),i.splice(i.indexOf(p),1),p.children.forEach(l),p.alias.forEach(l))}else{const p=i.indexOf(h);p>-1&&(i.splice(p,1),h.record.name&&s.delete(h.record.name),h.children.forEach(l),h.alias.forEach(l))}}function o(){return i}function a(h){let p=0;for(;p<i.length&&oc(h,i[p])>=0&&(h.record.path!==i[p].record.path||!Ol(h,i[p]));)p++;i.splice(p,0,h),h.record.name&&!U2(h)&&s.set(h.record.name,h)}function c(h,p){let d,g={},x,A;if("name"in h&&h.name){if(d=s.get(h.name),!d)throw St(1,{location:h});A=d.record.name,g=fe(pc(p.params,d.keys.filter(z=>!z.optional).map(z=>z.name)),h.params),x=d.stringify(g)}else if("path"in h)x=h.path,d=i.find(z=>z.re.test(x)),d&&(g=d.parse(x),A=d.record.name);else{if(d=p.name?s.get(p.name):i.find(z=>z.re.test(p.path)),!d)throw St(1,{location:h,currentLocation:p});A=d.record.name,g=fe({},p.params,h.params),x=d.stringify(g)}const C=[];let D=d;for(;D;)C.unshift(D.record),D=D.parent;return{name:A,path:x,params:g,matched:C,meta:gc(C)}}return t.forEach(h=>r(h)),{addRoute:r,resolve:c,removeRoute:l,getRoutes:o,getRecordMatcher:n}}function pc(t,e){const i={};for(const s of e)s in t&&(i[s]=t[s]);return i}function uc(t){return{path:t.path,redirect:t.redirect,name:t.name,meta:t.meta||{},aliasOf:void 0,beforeEnter:t.beforeEnter,props:mc(t),children:t.children||[],instances:{},leaveGuards:new Set,updateGuards:new Set,enterCallbacks:{},components:"components"in t?t.components||{}:{default:t.component}}}function mc(t){const e={},i=t.props||!1;if("component"in t)e.default=i;else for(const s in t.components)e[s]=typeof i=="boolean"?i:i[s];return e}function U2(t){for(;t;){if(t.record.aliasOf)return!0;t=t.parent}return!1}function gc(t){return t.reduce((e,i)=>fe(e,i.meta),{})}function V2(t,e){const i={};for(const s in t)i[s]=s in e?e[s]:t[s];return i}function Ol(t,e){return e.children.some(i=>i===t||Ol(t,i))}const Rl=/#/g,yc=/&/g,xc=/\//g,vc=/=/g,wc=/\?/g,Tl=/\+/g,bc=/%5B/g,kc=/%5D/g,Ml=/%5E/g,zc=/%60/g,Fl=/%7B/g,_c=/%7C/g,Il=/%7D/g,Cc=/%20/g;function an(t){return encodeURI(""+t).replace(_c,"|").replace(bc,"[").replace(kc,"]")}function $c(t){return an(t).replace(Fl,"{").replace(Il,"}").replace(Ml,"^")}function $s(t){return an(t).replace(Tl,"%2B").replace(Cc,"+").replace(Rl,"%23").replace(yc,"%26").replace(zc,"`").replace(Fl,"{").replace(Il,"}").replace(Ml,"^")}function Ec(t){return $s(t).replace(vc,"%3D")}function Sc(t){return an(t).replace(Rl,"%23").replace(wc,"%3F")}function Pc(t){return t==null?"":Sc(t).replace(xc,"%2F")}function ii(t){try{return decodeURIComponent(""+t)}catch{}return""+t}function Dc(t){const e={};if(t===""||t==="?")return e;const s=(t[0]==="?"?t.slice(1):t).split("&");for(let n=0;n<s.length;++n){const r=s[n].replace(Tl," "),l=r.indexOf("="),o=ii(l<0?r:r.slice(0,l)),a=l<0?null:ii(r.slice(l+1));if(o in e){let c=e[o];Array.isArray(c)||(c=e[o]=[c]),c.push(a)}else e[o]=a}return e}function G2(t){let e="";for(let i in t){const s=t[i];if(i=Ec(i),s==null){s!==void 0&&(e+=(e.length?"&":"")+i);continue}(Array.isArray(s)?s.map(r=>r&&$s(r)):[s&&$s(s)]).forEach(r=>{r!==void 0&&(e+=(e.length?"&":"")+i,r!=null&&(e+="="+r))})}return e}function Ac(t){const e={};for(const i in t){const s=t[i];s!==void 0&&(e[i]=Array.isArray(s)?s.map(n=>n==null?null:""+n):s==null?s:""+s)}return e}function Nt(){let t=[];function e(s){return t.push(s),()=>{const n=t.indexOf(s);n>-1&&t.splice(n,1)}}function i(){t=[]}return{add:e,list:()=>t,reset:i}}function C0(t,e,i,s,n){const r=s&&(s.enterCallbacks[n]=s.enterCallbacks[n]||[]);return()=>new Promise((l,o)=>{const a=p=>{p===!1?o(St(4,{from:i,to:e})):p instanceof Error?o(p):ic(p)?o(St(2,{from:e,to:p})):(r&&s.enterCallbacks[n]===r&&typeof p=="function"&&r.push(p),l())},c=t.call(s&&s.instances[n],e,i,a);let h=Promise.resolve(c);t.length<3&&(h=h.then(a)),h.catch(p=>o(p))})}function Wi(t,e,i,s){const n=[];for(const r of t)for(const l in r.components){let o=r.components[l];if(!(e!=="beforeRouteEnter"&&!r.instances[l]))if(Oc(o)){const c=(o.__vccOpts||o)[e];c&&n.push(C0(c,i,s,r,l))}else{let a=o();n.push(()=>a.then(c=>{if(!c)return Promise.reject(new Error(`Couldn't resolve component "${l}" at "${r.path}"`));const h=j3(c)?c.default:c;r.components[l]=h;const d=(h.__vccOpts||h)[e];return d&&C0(d,i,s,r,l)()}))}}return n}function Oc(t){return typeof t=="object"||"displayName"in t||"props"in t||"__vccOpts"in t}function q2(t){const e=P0(on),i=P0(El),s=o0(()=>e.resolve(Gt(t.to))),n=o0(()=>{const{matched:a}=s.value,{length:c}=a,h=a[c-1],p=i.matched;if(!h||!p.length)return-1;const d=p.findIndex(Et.bind(null,h));if(d>-1)return d;const g=K2(a[c-2]);return c>1&&K2(h)===g&&p[p.length-1].path!==g?p.findIndex(Et.bind(null,a[c-2])):d}),r=o0(()=>n.value>-1&&Fc(i.params,s.value.params)),l=o0(()=>n.value>-1&&n.value===i.matched.length-1&&Sl(i.params,s.value.params));function o(a={}){return Mc(a)?e[Gt(t.replace)?"replace":"push"](Gt(t.to)).catch(Jt):Promise.resolve()}return{route:s,href:o0(()=>s.value.href),isActive:r,isExactActive:l,navigate:o}}const Rc=Us({name:"RouterLink",props:{to:{type:[String,Object],required:!0},replace:Boolean,activeClass:String,exactActiveClass:String,custom:Boolean,ariaCurrentValue:{type:String,default:"page"}},useLink:q2,setup(t,{slots:e}){const i=Q0(q2(t)),{options:s}=P0(on),n=o0(()=>({[X2(t.activeClass,s.linkActiveClass,"router-link-active")]:i.isActive,[X2(t.exactActiveClass,s.linkExactActiveClass,"router-link-exact-active")]:i.isExactActive}));return()=>{const r=e.default&&e.default(i);return t.custom?r:Ys("a",{"aria-current":i.isExactActive?t.ariaCurrentValue:null,href:i.href,onClick:i.navigate,class:n.value},r)}}}),Tc=Rc;function Mc(t){if(!(t.metaKey||t.altKey||t.ctrlKey||t.shiftKey)&&!t.defaultPrevented&&!(t.button!==void 0&&t.button!==0)){if(t.currentTarget&&t.currentTarget.getAttribute){const e=t.currentTarget.getAttribute("target");if(/\b_blank\b/i.test(e))return}return t.preventDefault&&t.preventDefault(),!0}}function Fc(t,e){for(const i in e){const s=e[i],n=t[i];if(typeof s=="string"){if(s!==n)return!1}else if(!Array.isArray(n)||n.length!==s.length||s.some((r,l)=>r!==n[l]))return!1}return!0}function K2(t){return t?t.aliasOf?t.aliasOf.path:t.path:""}const X2=(t,e,i)=>t!=null?t:e!=null?e:i,Ic=Us({name:"RouterView",inheritAttrs:!1,props:{name:{type:String,default:"default"},route:Object},compatConfig:{MODE:3},setup(t,{attrs:e,slots:i}){const s=P0(_s),n=o0(()=>t.route||s.value),r=P0(F2,0),l=o0(()=>n.value.matched[r]);j1(F2,r+1),j1(I3,l),j1(_s,n);const o=Ls();return Xt(()=>[o.value,l.value,t.name],([a,c,h],[p,d,g])=>{c&&(c.instances[h]=a,d&&d!==c&&a&&a===p&&(c.leaveGuards.size||(c.leaveGuards=d.leaveGuards),c.updateGuards.size||(c.updateGuards=d.updateGuards))),a&&c&&(!d||!Et(c,d)||!p)&&(c.enterCallbacks[h]||[]).forEach(x=>x(a))},{flush:"post"}),()=>{const a=n.value,c=l.value,h=c&&c.components[t.name],p=t.name;if(!h)return W2(i.default,{Component:h,route:a});const d=c.props[t.name],g=d?d===!0?a.params:typeof d=="function"?d(a):d:null,A=Ys(h,fe({},g,e,{onVnodeUnmounted:C=>{C.component.isUnmounted&&(c.instances[p]=null)},ref:o}));return W2(i.default,{Component:A,route:a})||A}}});function W2(t,e){if(!t)return null;const i=t(e);return i.length===1?i[0]:i}const jc=Ic;function Bc(t){const e=dc(t.routes,t),i=t.parseQuery||Dc,s=t.stringifyQuery||G2,n=t.history,r=Nt(),l=Nt(),o=Nt(),a=P4(w0);let c=w0;pt&&t.scrollBehavior&&"scrollRestoration"in history&&(history.scrollRestoration="manual");const h=Ki.bind(null,v=>""+v),p=Ki.bind(null,Pc),d=Ki.bind(null,ii);function g(v,F){let S,I;return Dl(v)?(S=e.getRecordMatcher(v),I=F):I=v,e.addRoute(I,S)}function x(v){const F=e.getRecordMatcher(v);F&&e.removeRoute(F)}function A(){return e.getRoutes().map(v=>v.record)}function C(v){return!!e.getRecordMatcher(v)}function D(v,F){if(F=fe({},F||a.value),typeof v=="string"){const Z=Xi(i,v,F.path),f=e.resolve({path:Z.path},F),u=n.createHref(Z.fullPath);return fe(Z,f,{params:d(f.params),hash:ii(Z.hash),redirectedFrom:void 0,href:u})}let S;if("path"in v)S=fe({},v,{path:Xi(i,v.path,F.path).path});else{const Z=fe({},v.params);for(const f in Z)Z[f]==null&&delete Z[f];S=fe({},v,{params:p(v.params)}),F.params=p(F.params)}const I=e.resolve(S,F),ae=v.hash||"";I.params=h(d(I.params));const ue=L3(s,fe({},v,{hash:$c(ae),path:I.path})),ee=n.createHref(ue);return fe({fullPath:ue,hash:ae,query:s===G2?Ac(v.query):v.query||{}},I,{redirectedFrom:void 0,href:ee})}function z(v){return typeof v=="string"?Xi(i,v,a.value.path):fe({},v)}function O(v,F){if(c!==v)return St(8,{from:F,to:v})}function P(v){return K(v)}function W(v){return P(fe(z(v),{replace:!0}))}function J(v){const F=v.matched[v.matched.length-1];if(F&&F.redirect){const{redirect:S}=F;let I=typeof S=="function"?S(v):S;return typeof I=="string"&&(I=I.includes("?")||I.includes("#")?I=z(I):{path:I},I.params={}),fe({query:v.query,hash:v.hash,params:v.params},I)}}function K(v,F){const S=c=D(v),I=a.value,ae=v.state,ue=v.force,ee=v.replace===!0,Z=J(S);if(Z)return K(fe(z(Z),{state:ae,force:ue,replace:ee}),F||S);const f=S;f.redirectedFrom=F;let u;return!ue&&H3(s,I,S)&&(u=St(16,{to:f,from:I}),lt(I,I,!0,!1)),(u?Promise.resolve(u):Q(f,I)).catch(m=>b0(m)?b0(m,2)?m:Ie(m):pe(m,f,I)).then(m=>{if(m){if(b0(m,2))return K(fe(z(m.to),{state:ae,force:ue,replace:ee}),F||f)}else m=le(f,I,!0,ee,ae);return ie(f,I,m),m})}function N(v,F){const S=O(v,F);return S?Promise.reject(S):Promise.resolve()}function Q(v,F){let S;const[I,ae,ue]=Nc(v,F);S=Wi(I.reverse(),"beforeRouteLeave",v,F);for(const Z of I)Z.leaveGuards.forEach(f=>{S.push(C0(f,v,F))});const ee=N.bind(null,v,F);return S.push(ee),ct(S).then(()=>{S=[];for(const Z of r.list())S.push(C0(Z,v,F));return S.push(ee),ct(S)}).then(()=>{S=Wi(ae,"beforeRouteUpdate",v,F);for(const Z of ae)Z.updateGuards.forEach(f=>{S.push(C0(f,v,F))});return S.push(ee),ct(S)}).then(()=>{S=[];for(const Z of v.matched)if(Z.beforeEnter&&!F.matched.includes(Z))if(Array.isArray(Z.beforeEnter))for(const f of Z.beforeEnter)S.push(C0(f,v,F));else S.push(C0(Z.beforeEnter,v,F));return S.push(ee),ct(S)}).then(()=>(v.matched.forEach(Z=>Z.enterCallbacks={}),S=Wi(ue,"beforeRouteEnter",v,F),S.push(ee),ct(S))).then(()=>{S=[];for(const Z of l.list())S.push(C0(Z,v,F));return S.push(ee),ct(S)}).catch(Z=>b0(Z,8)?Z:Promise.reject(Z))}function ie(v,F,S){for(const I of o.list())I(v,F,S)}function le(v,F,S,I,ae){const ue=O(v,F);if(ue)return ue;const ee=F===w0,Z=pt?history.state:{};S&&(I||ee?n.replace(v.fullPath,fe({scroll:ee&&Z&&Z.scroll},ae)):n.push(v.fullPath,ae)),a.value=v,lt(v,F,S,ee),Ie()}let T;function me(){T||(T=n.listen((v,F,S)=>{const I=D(v),ae=J(I);if(ae){K(fe(ae,{replace:!0}),I).catch(Jt);return}c=I;const ue=a.value;pt&&Y3(B2(ue.fullPath,S.delta),Ei()),Q(I,ue).catch(ee=>b0(ee,12)?ee:b0(ee,2)?(K(ee.to,I).then(Z=>{b0(Z,20)&&!S.delta&&S.type===y1.pop&&n.go(-1,!1)}).catch(Jt),Promise.reject()):(S.delta&&n.go(-S.delta,!1),pe(ee,I,ue))).then(ee=>{ee=ee||le(I,ue,!1),ee&&(S.delta?n.go(-S.delta,!1):S.type===y1.pop&&b0(ee,20)&&n.go(-1,!1)),ie(I,ue,ee)}).catch(Jt)}))}let Ae=Nt(),h0=Nt(),ye;function pe(v,F,S){Ie(v);const I=h0.list();return I.length?I.forEach(ae=>ae(v,F,S)):console.error(v),Promise.reject(v)}function oe(){return ye&&a.value!==w0?Promise.resolve():new Promise((v,F)=>{Ae.add([v,F])})}function Ie(v){return ye||(ye=!v,me(),Ae.list().forEach(([F,S])=>v?S(v):F()),Ae.reset()),v}function lt(v,F,S,I){const{scrollBehavior:ae}=t;if(!pt||!ae)return Promise.resolve();const ue=!S&&Z3(B2(v.fullPath,0))||(I||!S)&&history.state&&history.state.scroll||null;return kr().then(()=>ae(v,F,ue)).then(ee=>ee&&W3(ee)).catch(ee=>pe(ee,v,F))}const f0=v=>n.go(v);let e0;const Le=new Set;return{currentRoute:a,addRoute:g,removeRoute:x,hasRoute:C,getRoutes:A,resolve:D,options:t,push:P,replace:W,go:f0,back:()=>f0(-1),forward:()=>f0(1),beforeEach:r.add,beforeResolve:l.add,afterEach:o.add,onError:h0.add,isReady:oe,install(v){const F=this;v.component("RouterLink",Tc),v.component("RouterView",jc),v.config.globalProperties.$router=F,Object.defineProperty(v.config.globalProperties,"$route",{enumerable:!0,get:()=>Gt(a)}),pt&&!e0&&a.value===w0&&(e0=!0,P(n.location).catch(ae=>{}));const S={};for(const ae in w0)S[ae]=o0(()=>a.value[ae]);v.provide(on,F),v.provide(El,Q0(S)),v.provide(_s,a);const I=v.unmount;Le.add(v),v.unmount=function(){Le.delete(v),Le.size<1&&(c=w0,T&&T(),T=null,a.value=w0,e0=!1,ye=!1),I()}}}}function ct(t){return t.reduce((e,i)=>e.then(()=>i()),Promise.resolve())}function Nc(t,e){const i=[],s=[],n=[],r=Math.max(e.matched.length,t.matched.length);for(let l=0;l<r;l++){const o=e.matched[l];o&&(t.matched.find(c=>Et(c,o))?s.push(o):i.push(o));const a=t.matched[l];a&&(e.matched.find(c=>Et(c,a))||n.push(a))}return[i,s,n]}const Lc=function(){const e=document.createElement("link").relList;return e&&e.supports&&e.supports("modulepreload")?"modulepreload":"preload"}(),Y2={},Hc="/",Yi=function(e,i){return!i||i.length===0?e():Promise.all(i.map(s=>{if(s=`${Hc}${s}`,s in Y2)return;Y2[s]=!0;const n=s.endsWith(".css"),r=n?'[rel="stylesheet"]':"";if(document.querySelector(`link[href="${s}"]${r}`))return;const l=document.createElement("link");if(l.rel=n?"stylesheet":Lc,n||(l.as="script",l.crossOrigin=""),l.href=s,document.head.appendChild(l),n)return new Promise((o,a)=>{l.addEventListener("load",o),l.addEventListener("error",()=>a(new Error(`Unable to preload CSS for ${s}`)))})})).then(()=>e())},Uc=[{path:"/",component:()=>Yi(()=>import("./MainLayout.765e2c7c.js"),["assets/MainLayout.765e2c7c.js","assets/dom.211ddf29.js","assets/QResizeObserver.5d0bd34f.js"]),children:[{path:"",component:()=>Yi(()=>import("./IndexPage.d865328e.js"),["assets/IndexPage.d865328e.js","assets/IndexPage.536753c7.css","assets/QBtn.3a9df91c.js","assets/dom.211ddf29.js","assets/QResizeObserver.5d0bd34f.js"])}]},{path:"/:catchAll(.*)*",component:()=>Yi(()=>import("./ErrorNotFound.d77032ee.js"),["assets/ErrorNotFound.d77032ee.js","assets/QBtn.3a9df91c.js","assets/dom.211ddf29.js"])}];var Zi=function(){return Bc({scrollBehavior:()=>({left:0,top:0}),routes:Uc,history:tc("/")})};async function Vc(t,e){const i=t(O3);i.use(Ha,e);const s=typeof qi=="function"?await qi({}):qi;i.use(s);const n=ai(typeof Zi=="function"?await Zi({store:s}):Zi);return s.use(({store:r})=>{r.router=n}),{app:i,store:s,router:n}}var Gc={config:{}};async function qc({app:t,router:e,store:i}){t.use(e),t.mount("#q-app")}Vc(ga,Gc).then(qc);export{r5 as $,f5 as A,o5 as B,Wn as C,c5 as D,h5 as E,Qn as F,Ye as G,n5 as H,sl as I,u5 as J,za as K,Zs as L,eo as M,Z4 as N,Y4 as O,ms as P,Aa as Q,l5 as R,s5 as S,Jc as T,a5 as U,g5 as V,e2 as W,m5 as X,Wc as Y,Sa as Z,P3 as _,p5 as a,re as a0,Qc as a1,Jr as a2,Ps as a3,e5 as a4,Xc as a5,Ss as a6,Oo as a7,t5 as a8,Zc as a9,r0 as aa,i5 as ab,Ao as ac,Gs as b,o0 as c,vt as d,Y0 as e,Q0 as f,Qr as g,Ys as h,P0 as i,jr as j,Us as k,d5 as l,Xr as m,l1 as n,Vs as o,j1 as p,Yr as q,Ls as r,N4 as s,ro as t,Me as u,ai as v,Xt as w,Yc as x,Se as y,kr as z};
