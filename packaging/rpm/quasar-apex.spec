Name:           quasar-apex
Version:        10.0.0
Release:        1%{?dist}
Summary:        High-performance portable C++ text search toolkit
License:        Apache-2.0
URL:            https://github.com/OWNER/REPO
Source0:        quasar-apex-%{version}.tar.gz

BuildRequires:  cmake gcc-c++ ninja-build

%description
High-performance portable C++ text search toolkit.

%prep
%setup -q

%build
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build -j

%install
DESTDIR=%{buildroot} cmake --install build

%files
%license LICENSE
%doc README.md
/usr/local/*
