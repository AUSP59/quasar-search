class QuasarApex < Formula
  desc "High-performance portable C++ text search toolkit"
  homepage "https://github.com/OWNER/REPO"
  url "https://github.com/OWNER/REPO/releases/download/v10.0.0/quasar-apex-10.0.0.tar.gz"
  sha256 "REPLACE_WITH_REAL_SHASUM"
  license "Apache-2.0"

  def install
    system "cmake", "-S", ".", "-B", "build", "-D", "CMAKE_BUILD_TYPE=Release"
    system "cmake", "--build", "build", "-j"
    system "cmake", "--install", "build", "--prefix", prefix
  end

  test do
    system "#<built-in function bin>/quasar-grep", "--help"
  end
end
